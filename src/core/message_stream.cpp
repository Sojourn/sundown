#include "core.h"

using namespace Sundown;

namespace {
    struct MessageFrame
    {
        std::atomic_uint_fast64_t size;
        alignas(alignof(uint64_t)) uint8_t buffer[sizeof(uint64_t)];
    };
    static_assert(sizeof(MessageFrame) == (sizeof(uint64_t) * 2), "Check packing");

    uint64_t frameSize(uint64_t size)
    {
        return sizeof(MessageFrame::size) + sizeof(uint64_t) - ((sizeof(uint64_t) - 1) & size);
    }

    uint64_t nextFrame(uint64_t prev, uint64_t size)
    {
        return prev + frameSize(size);
    }
}

Message::Message()
{
    reset();
}

Message::Message(uint64_t begin, uint64_t end, uint64_t size)
    : begin_(begin)
    , end_(end)
    , size_(size)
{
}

Message::Message(Message &&other)
{
    begin_ = other.begin_;
    end_ = other.end_;
    size_ = other.size_;
    other.reset();
}

Message::~Message()
{
    assert(empty());
}

Message &Message::operator=(Message &&rhs)
{
    if (this != &rhs) {
        assert(empty());

        begin_ = rhs.begin_;
        end_ = rhs.end_;
        size_ = rhs.size_;
        rhs.reset();
    }

    return *this;
}

Message::operator bool() const
{
    return !empty();
}

bool Message::empty() const
{
    return begin_ == end_;
}

void Message::reset()
{
    begin_ = 0;
    end_ = 0;
    size_ = 0;
}

MessageStreamCursor::MessageStreamCursor(std::shared_ptr<MessageStream> stream)
    : stream_(std::move(stream))
{
    Reflector &reflector = stream_->reflector_;
    size_ = reflector.size();
    mask_ = size_ - 1;
    base_ = reflector.lower();
}

uint8_t *MessageStreamCursor::addr(uint64_t offset)
{
    return static_cast<uint8_t *>(base_) + (offset & mask_);
}

const uint8_t *MessageStreamCursor::addr(uint64_t offset) const
{
    return static_cast<const uint8_t *>(base_) + (offset & mask_);
}

uint64_t MessageStreamCursor::mask() const
{
    return mask_;
}

uint64_t MessageStreamCursor::size() const
{
    return size_;
}

MessageStream &MessageStreamCursor::stream()
{
    return *stream_;
}

const MessageStream &MessageStreamCursor::stream() const
{
    return *stream_;
}

MessageStreamWriter::MessageStreamWriter(std::shared_ptr<MessageStream> stream)
    : cursor_(std::move(stream))
    , head_(cursor_.size())
{
}

Message MessageStreamWriter::claim(size_t size)
{
    uint64_t begin;
    uint64_t end;
    uint64_t len;

    len = frameSize(size);
    begin = cursor_.stream().writer_.fetch_add(len, std::memory_order_relaxed);
    end = begin + len;

    return Message(begin, end, size);
}

Optional<Buffer> MessageStreamWriter::prepare(const Message &message)
{
    assert(message);

    if (readyFast(message) || readySlow(message)) {
        uint8_t *begin = cursor_.addr(message.begin_) + sizeof(MessageFrame::size);
        uint8_t *end = begin + message.size_;
        return Optional<Buffer>(Buffer(begin, end));
    }
    else {
        return None();
    }
}

bool MessageStreamWriter::readyFast(const Message &message)
{
    return head_ > message.end_;
}

bool MessageStreamWriter::readySlow(const Message &message)
{
    uint64_t reader = cursor_.stream().reader_.load();
    head_ = reader + cursor_.size(); // This doesn't seem right...

    return readyFast(message);
}

void MessageStreamWriter::commit(Message message)
{
    assert(message);

    MessageFrame &frame = reinterpret_cast<MessageFrame &>(*cursor_.addr(message.begin_));
    frame.size.store(message.size_);
}

MessageStreamReader::MessageStreamReader(std::shared_ptr<MessageStream> stream)
    : cursor_(std::move(stream))
    , head_(0)
    , tail_(0)
{
}

Optional<Buffer> MessageStreamReader::read()
{
    clean();

    MessageFrame &frame = reinterpret_cast<MessageFrame &>(*cursor_.addr(head_));
    uint64_t messageSize = frame.size.load();
    if (messageSize > 0) {
        head_ += frameSize(messageSize);
        return Optional<Buffer>(Buffer(frame.buffer, frame.buffer + messageSize));
    }
    else {
        return None();
    }
}

void MessageStreamReader::clean()
{
    assert(head_ >= tail_);

    uint64_t dirty = head_ - tail_;
    if (dirty > 4096) {
        std::memset(cursor_.addr(tail_), 0, dirty);
        tail_ = head_;
        cursor_.stream().reader_.store(tail_);
    }
}

Optional<std::shared_ptr<MessageStream>> MessageStream::create(size_t capacity)
{
    Optional<Reflector> reflector = Reflector::create(capacity);
    if (reflector.empty()) {
        return None();
    }
    else {
        auto result = std::make_shared<MessageStream>(reflector.get());
        return Optional<std::shared_ptr<MessageStream>>(std::move(result));
    }
}

MessageStream::MessageStream(Reflector reflector)
    : reflector_(std::move(reflector))
{
    std::memset(reflector_.data(), 0, reflector_.size());
}

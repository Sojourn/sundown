#include "core.h"

using namespace Sundown;

Message::Message()
{
    reset();
}

Message::Message(uint64_t begin, uint64_t end)
    : begin_(begin)
    , end_(end)
{
}

Message::Message(Message &&other)
{
    begin_ = other.begin_;
    end_ = other.end_;
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
    , head_(0)
    , tail_(0)
{
}

Message MessageStreamWriter::claim(size_t size)
{
    return Message();
}

Optional<Slice> MessageStreamWriter::prepare(Message message)
{
    return None();
}

void MessageStreamWriter::commit(Message message)
{
}

MessageStreamReader::MessageStreamReader(std::shared_ptr<MessageStream> stream)
    : cursor_(std::move(stream))
    , head_(0)
    , tail_(0)
{
}

Optional<Slice> MessageStreamReader::read()
{
    return None();
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
}

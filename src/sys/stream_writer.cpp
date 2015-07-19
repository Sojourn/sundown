#include "sys.h"

using namespace Sundown;

Optional<StreamWriter> StreamWriter::create()
{
    Optional<Reflector> reflector = Reflector::create(1 << 16);
    if (reflector.empty()) {
        return Optional<StreamWriter>();
    }
    else {
        return StreamWriter(reflector.get());
    }
}

StreamWriter::StreamWriter(Reflector reflector)
    : mask_(reflector.size() - 1)
    , writer_(0)
    , reader_(0)
    , pending_(0)
    , reflector_(std::move(reflector))
{
}

void *StreamWriter::begin(size_t size)
{
    assert(size > 0);
    assert(pending_ == 0);

    if (backlog_.empty()) {
        if (reflector_.size() - (writer_ - reader_)) {
            uint8_t *ptr = static_cast<uint8_t *>(reflector_.lower());
            ptr += writer_ & mask_;
            pending_ = size;
            writer_ += size;
            return ptr;
        }
    }

    size_t prev = backlog_.size();
    backlog_.resize(backlog_.size() + size);
    pending_ = size;
    return &backlog_[prev];
}

void StreamWriter::commit()
{
    assert(pending_ >= 0);

    pending_ = 0;
}

void StreamWriter::rollback()
{
    assert(pending_ >= 0);

    if (backlog_.empty()) {
        writer_ -= pending_;
        pending_ = 0;
    }
    else {
        backlog_.erase(backlog_.begin() + (backlog_.size() - pending_), backlog_.end());
        pending_ = 0;
    }
}

void StreamWriter::flush(const FileDescriptor &fd)
{
    assert(pending_ == 0);

    if (writer_ - reader_) {
        off_t offset = reader_;
        size_t count;
        if ((writer_ & ~mask_) == (reader_ & ~mask_)) {
            count = writer_ - reader_;
        }
        else {
            count = ((reader_ & ~mask_) + reflector_.size()) - reader_;
        }

        ssize_t result = sendfile(fd.get(), reflector_.fd().get(), &offset, count);
        if (result < 0) {
            assert(false);
        }
        else {
            reader_ += result;
        }
    }
    else {
        ssize_t result = write(fd.get(), backlog_.data(), backlog_.size());
        if (result < 0) {
            assert(false);
        }
        else {
            backlog_.erase(backlog_.begin(), backlog_.begin() + result);
        }
    }
}

size_t StreamWriter::writable() const
{
    if (backlog_.empty()) {
        return reflectorSpace();
    }
    else {
        return 0;
    }
}

size_t StreamWriter::backlog() const
{
    return (writer_ - reader_) + backlog_.size();
}

void *StreamWriter::reflectorData(uint64_t offset)
{
    return static_cast<uint8_t *>(reflector_.data()) + (offset & mask_);
}

size_t StreamWriter::reflectorSpace() const
{
    return reflector_.size() - (writer_ - reader_);
}


#include "mem.h"

using namespace Sundown;

Buffer::Buffer(uint8_t *first, uint8_t *last)
    : first_(first)
    , last_(last)
{
    assert(first_ <= last_);
}

uint8_t *Buffer::begin()
{
    return first_;
}

const uint8_t *Buffer::begin() const
{
    return first_;
}

uint8_t *Buffer::end()
{
    return last_;
}

const uint8_t *Buffer::end() const
{
    return last_;
}

size_t Buffer::size() const
{
    return last_ - first_;
}

bool Buffer::empty() const
{
    return first_ != last_;
}


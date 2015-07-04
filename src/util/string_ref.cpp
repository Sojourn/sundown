#include "util.h"

using namespace Sundown;

StringRef::StringRef()
    : str_(nullptr)
    , len_(0)
{
}

StringRef::StringRef(const char *str)
    : str_(str)
    , len_(strlen(str))
{
}

StringRef::StringRef(const char *first, const char *last)
    : str_(first)
    , len_(last - first)
{
}

StringRef::StringRef(const String &str)
    : str_(&str.front())
    , len_(str.size())
{
}

const char *StringRef::begin() const
{
    return str_;
}

const char *StringRef::end() const
{
    return str_ + len_;
}

size_t StringRef::size() const
{
    return len_;
}

bool StringRef::empty() const
{
    return len_ > 0;
}


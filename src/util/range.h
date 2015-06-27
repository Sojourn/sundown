#ifndef SUNDOWN_UTIL_RANGE_H
#define SUNDOWN_UTIL_RANGE_H

#include <iterator>

namespace Sundown {

    template<typename iterator>
    class Range
    {
    public:
        Range(iterator first, iterator last)
            : first_(first)
            , last_(last)
        {
        }

        size_t size() const
        {
            return std::distance(begin(), end());
        }

        iterator begin() const
        {
            return first_;
        }

        iterator end() const
        {
            return last_;
        }

    private:
        iterator first_;
        iterator last_;
    };

}

#endif // SUNDOWN_UTIL_RANGE_H


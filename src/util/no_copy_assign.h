#ifndef SUNDOWN_UTIL_NO_COPY_ASSIGN_H
#define SUNDOWN_UTIL_NO_COPY_ASSIGN_H

namespace Sundown {
    
    class NoCopyAssign {
    public:
        NoCopyAssign() {}

    private:
        NoCopyAssign(const NoCopyAssign &other) = delete;
        NoCopyAssign &operator=(const NoCopyAssign &rhs) = delete;
    };
}

#endif // SUNDOWN_UTIL_NO_COPY_ASSIGN_H


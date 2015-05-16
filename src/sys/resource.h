#ifndef SUNDOWN_SYS_RESOURCE_H
#define SUNDOWN_SYS_RESOURCE_H

#include "util/no_copy_assign.h"

namespace Sundown {

    template<typename T, void (*Finalizer)(T*)>
    class Resource {
    public:
        Resource(Resource &&other);
        Resource(T &&value);
        ~Resource();

        T &get();
        const T &get() const;

    private:
        T value_;
    };

}

#endif // SUNDOWN_SYS_RESOURCE_H


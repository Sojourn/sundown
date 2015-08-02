#ifndef SUNDOWN_UTIL_MACROS_H
#define SUNDOWN_UTIL_MACROS_H

#include <memory>

// Make the compiler sad
#define SUNDOWN_DYNAMIC_TRUE (std::make_shared<int>(3).get() != nullptr)
#define SUNDOWN_DYNAMIC_FALSE (!SUNDOWN_DYNAMIC_TRUE)

#endif // SUNDOWN_UTIL_MACROS_H


#ifndef SUNDOWN_SYS_H
#define SUNDOWN_SYS_H

#include <cstdint>
#include <cassert>

#include "sys/resource.h"
#include "sys/error.h"

#if SUNDOWN_OS_POSIX
#elif SUNDOWN_OS_WIN32
#else
#  error "Unknown OS"
#endif

#endif // SUNDOWN_SYS_H


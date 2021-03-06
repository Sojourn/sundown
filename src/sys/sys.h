#ifndef SUNDOWN_SYS_H
#define SUNDOWN_SYS_H

#include <array>
#include <vector>
#include <memory>

#include <cstdio>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cassert>

#if SUNDOWN_OS_POSIX

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <unistd.h>
#include <fcntl.h>

#elif SUNDOWN_OS_WIN32
#else
#  error "Unknown OS"
#endif

#include "util/util.h"

#include "sys/fd.h"
#include "sys/pipe.h"
#include "sys/selector.h"
#include "sys/reflector.h"
#include "sys/stream_writer.h"

#endif // SUNDOWN_SYS_H


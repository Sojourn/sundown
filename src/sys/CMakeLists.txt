set(SYS_HDRS
    sys.h
    resource.h
)

set(SYS_SRCS
    sys.cpp
)

if(SUNDOWN_OS_POSIX)
set(SYS_HDRS
    ${SYS_HDRS}
)
set(SYS_SRCS
    ${SYS_SRCS}
)
elseif(SUNDOWN_OS_WIN32)
set(SYS_HDRS
    ${SYS_HDRS}
)
set(SYS_SRCS
    ${SYS_SRCS}
)
endif()

add_library(sundown_sys STATIC ${SYS_SRCS})


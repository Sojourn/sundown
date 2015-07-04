set(SYS_HDRS
    sys.h
    fd.h
    selector.h
    reflector.cpp
)

set(SYS_SRCS
    sys.cpp
    fd.cpp
    selector.cpp
    reflector.cpp
)

add_library(sundown_sys STATIC ${SYS_SRCS})


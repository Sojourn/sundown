set(SYS_HDRS
    sys.h
    fd.h
    pipe.h
    selector.h
    reflector.h
    stream_writer.h
)

set(SYS_SRCS
    sys.cpp
    fd.cpp
    pipe.cpp
    selector.cpp
    reflector.cpp
    stream_writer.cpp
)

add_library(sundown_sys STATIC ${SYS_SRCS})


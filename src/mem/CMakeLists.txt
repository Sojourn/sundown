set(MEM_HDRS
    mem.h
    buffer.h
)

set(MEM_SRCS
    mem.cpp
    buffer.cpp
)

add_library(sundown_mem STATIC ${MEM_SRCS})


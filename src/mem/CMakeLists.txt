set(MEM_HDRS
    mem.h
    slice.h
)

set(MEM_SRCS
    mem.cpp
    slice.cpp
)

add_library(sundown_mem STATIC ${MEM_SRCS})


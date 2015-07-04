set(CORE_HDRS
    core.h
    dispatcher.h
)

set(CORE_SRCS
    core.cpp
    dispatcher.cpp
)

add_library(sundown_core STATIC ${CORE_SRCS})


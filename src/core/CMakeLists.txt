set(CORE_HDRS
    core.h
    dispatcher.h
    message_stream.h
)

set(CORE_SRCS
    core.cpp
    dispatcher.cpp
    message_stream.cpp
)

add_library(sundown_core STATIC ${CORE_SRCS})


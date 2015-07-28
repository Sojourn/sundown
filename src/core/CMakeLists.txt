set(CORE_HDRS
    core.h
    message_stream.h
)

set(CORE_SRCS
    core.cpp
    message_stream.cpp
)

add_library(sundown_core STATIC ${CORE_SRCS})

target_link_libraries(sundown_core sundown_mem)
target_link_libraries(sundown_core sundown_util)
target_link_libraries(sundown_core sundown_sys)


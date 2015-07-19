set(UTIL_HDRS
    util.h
    macros.h
    range.h
    string.h
    string_ref.h
    optional.h
)

set(UTIL_SRCS
    util.cpp
    string_ref.cpp
)

add_library(sundown_util STATIC ${UTIL_SRCS})


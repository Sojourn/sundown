set(TST_HDRS
)

set(TST_SRCS
    tst.cpp
    util/tst_optional.cpp
    util/tst_range.cpp
    util/tst_string.cpp
    util/tst_string_ref.cpp
)

add_executable(tst ${TST_SRCS})

target_link_libraries(tst sundown_mem)
target_link_libraries(tst sundown_util)
target_link_libraries(tst sundown_sys)
target_link_libraries(tst sundown_core)


# Windows doesn't support symlinks?
if(SUNDOWN_OS_POSIX)
    add_executable(tst tst.cpp)
elseif(SUNDOWN_OS_WIN32)
    add_executable(tst src/tst.cpp)
else()
    message(STATUS "Ignoring tst target")
endif()

target_link_libraries(tst sundown_mem)
target_link_libraries(tst sundown_util)
target_link_libraries(tst sundown_sys)
target_link_libraries(tst sundown_core)


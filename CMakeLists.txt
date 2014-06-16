cmake_minimum_required (VERSION 2.8)
project(libcbor)
include(CTest)


set(CMAKE_C_FLAGS "-std=c99")
set(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -pedantic -g -ggdb")
set(CMAKE_EXE_LINKER_FLAGS "-g")

enable_testing()
add_custom_target(test ${CMAKE_CTEST_COMMAND} -V)

set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")
set(MEMORYCHECK_COMMAND_OPTIONS "")

include_directories(src)
subdirs(src)
subdirs(demo)
subdirs(test)
cmake_minimum_required(VERSION 3.12)

# Project
project(tests VERSION 1.0 LANGUAGES CXX)

# Find packages
find_package(Catch2 CONFIG REQUIRED)

# Third party includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libataxx/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libnnue/src/)

# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the test executable
add_executable(
    tests

    main.cpp
    uai-moves.cpp
    uai-pos.cpp
    uai-setoption.cpp
    options-check.cpp
    options-combo.cpp
    options-spin.cpp
    options-string.cpp
    hashtable.cpp
    tryhard_sorter.cpp

    ../autaxx/options.cpp
    ../autaxx/protocol/uai/moves.cpp
    ../autaxx/protocol/uai/position.cpp
    ../autaxx/protocol/uai/setoption.cpp

    ../autaxx/search/tryhard/eval.cpp
)

set_property(TARGET tests PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE)

target_link_libraries(tests Catch2::Catch2 "${CMAKE_CURRENT_LIST_DIR}/../libataxx/build/static/libataxx.a")

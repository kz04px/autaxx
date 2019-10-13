cmake_minimum_required(VERSION 3.12)

# Project
project(Autaxx VERSION 1.0 LANGUAGES CXX)

# Find packages
find_package(Catch2 CONFIG REQUIRED)

# Base directory relative includes
include_directories(.)

# Third party includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/libataxx/src/)

# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the executable
add_executable(
    autaxx
    src/main.cpp
    src/options.cpp
    src/protocol/uai/go.cpp
    src/protocol/uai/isready.cpp
    src/protocol/uai/listen.cpp
    src/protocol/uai/moves.cpp
    src/protocol/uai/position.cpp
    src/protocol/uai/setoption.cpp
    src/protocol/uai/uainewgame.cpp
    src/protocol/uai/extension/display.cpp
    src/protocol/uai/extension/perft.cpp
    src/protocol/uai/extension/split.cpp
    src/search/search.cpp
    src/search/alphabeta/alphabeta.cpp
    src/search/alphabeta/eval.cpp
    src/search/alphabeta/root.cpp
    src/search/minimax/minimax.cpp
    src/search/minimax/eval.cpp
    src/search/minimax/root.cpp
)

# Add the test executable
add_executable(
    autaxx-test
    tests/main.cpp
    tests/uai-moves.cpp
    tests/uai-pos.cpp
    tests/uai-setoption.cpp
    tests/options-check.cpp
    tests/options-combo.cpp
    tests/options-spin.cpp
    tests/options-string.cpp
    #tests/search-minimax.cpp
    src/options.cpp
    src/protocol/uai/moves.cpp
    src/protocol/uai/position.cpp
    src/protocol/uai/setoption.cpp
    #src/search/alphabeta/alphabeta.cpp
    #src/search/alphabeta/root.cpp
    #src/search/minimax/minimax.cpp
    #src/search/minimax/root.cpp
    #src/search/random/root.cpp
)

set_property(TARGET autaxx-test PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE)

target_link_libraries(autaxx "${CMAKE_CURRENT_LIST_DIR}/libs/libataxx/build/static/libataxx.a")
target_link_libraries(autaxx-test Catch2::Catch2 "${CMAKE_CURRENT_LIST_DIR}/libs/libataxx/build/static/libataxx.a")

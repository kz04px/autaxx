cmake_minimum_required(VERSION 3.12)

# Project
project(Autaxx VERSION 1.0 LANGUAGES CXX)

# Packages
find_package(Threads REQUIRED)

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

# Add the executable
add_executable(
    autaxx

    main.cpp
    options.cpp
    # UAI protocol
    protocol/uai/go.cpp
    protocol/uai/isready.cpp
    protocol/uai/listen.cpp
    protocol/uai/moves.cpp
    protocol/uai/position.cpp
    protocol/uai/setoption.cpp
    protocol/uai/uainewgame.cpp
    protocol/uai/extension/display.cpp
    protocol/uai/extension/perft.cpp
    protocol/uai/extension/split.cpp
    # UGI protocol
    protocol/ugi/go.cpp
    protocol/ugi/isready.cpp
    protocol/ugi/listen.cpp
    protocol/ugi/moves.cpp
    protocol/ugi/position.cpp
    protocol/ugi/setoption.cpp
    protocol/ugi/uginewgame.cpp
    protocol/ugi/extension/display.cpp
    protocol/ugi/extension/perft.cpp
    protocol/ugi/extension/split.cpp

    search/search.cpp

    search/tryhard/search.cpp
    search/tryhard/eval.cpp
    search/tryhard/root.cpp

    search/nnue/search.cpp
    search/nnue/eval.cpp
    search/nnue/root.cpp

    search/mcts/eval.cpp
    search/mcts/root.cpp

    search/minimax/minimax.cpp
    search/minimax/eval.cpp
    search/minimax/root.cpp

    search/alphabeta/search.cpp
    search/alphabeta/eval.cpp
    search/alphabeta/root.cpp
)

target_link_libraries(autaxx Threads::Threads "${CMAKE_CURRENT_LIST_DIR}/../libataxx/build/static/libataxx.a")

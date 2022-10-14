cmake_minimum_required(VERSION 3.12)

project(Autaxx)

include(FetchContent)

FetchContent_Declare(
    libataxx
    GIT_REPOSITORY https://github.com/kz04px/libataxx
    GIT_TAG 8abcaab060615d5355f5f401a77ba92552e332b9
)

FetchContent_Declare(
    libnnue
    GIT_REPOSITORY https://github.com/kz04px/libnnue
    GIT_TAG c783a2e636735559d31d8abb7cae350d5e3b6ee0
)

FetchContent_MakeAvailable(libataxx)
FetchContent_MakeAvailable(libnnue)

# Packages
find_package(Threads REQUIRED)
find_package(Catch2 CONFIG)

add_subdirectory(src/autaxx)

if(Catch2_FOUND)
    add_subdirectory(src/tests)
endif()

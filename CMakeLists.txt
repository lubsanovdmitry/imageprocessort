cmake_minimum_required(VERSION 3.14...3.27)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(cmaketest1 VERSION 0.1.0 
        DESCRIPTION "cmake test"
        LANGUAGES CXX
)

# set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)

add_executable(image_processor)

add_subdirectory(src)
add_subdirectory(tests)

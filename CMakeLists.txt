# CMake minimum version required
cmake_minimum_required(VERSION 3.20)
include(FetchContent)

# Project name
project(NoarrTokenizer LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

if(NOT DEFINED NOARR_STRUCTURES_BRANCH)
  set(NOARR_STRUCTURES_BRANCH main)
endif()

# declare noarr dependency, url: https://github.com/jiriklepl/noarr-structures.git
FetchContent_Declare(
    Noarr
    GIT_REPOSITORY https://github.com/jiriklepl/noarr-structures.git
    GIT_TAG        ${NOARR_STRUCTURES_BRANCH})
FetchContent_MakeAvailable(Noarr)


# Add include directories
include_directories(include)
include_directories(${Noarr_SOURCE_DIR}/include)

# Add executable target
add_executable(test test.cpp)

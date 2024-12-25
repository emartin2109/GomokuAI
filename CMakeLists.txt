cmake_minimum_required(VERSION 3.16)
project(pbrain-gomoku-ai CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
option(BUILD_TESTS "Build tests" OFF)
add_subdirectory(third-party)

if (BUILD_TESTS)
    include(MakeTests)
endif()

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

cmake_minimum_required(VERSION 3.0)
project(CppLinAlg VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

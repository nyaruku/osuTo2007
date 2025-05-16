cmake_minimum_required(VERSION 3.15)
project(osuTo20078 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# STATIC LIB
option(USE_STATIC_LIB "Use static library" ON)

# Debug
message(STATUS "#################################################")
message(STATUS "#################################################")
message(STATUS "CMAKE_SYSROOT: ${CMAKE_SYSROOT}")
message(STATUS "BASE_INCLUDE_PATH: ${BASE_INCLUDE_PATH}")
message(STATUS "BASE_LIB_PATH: ${BASE_LIB_PATH}")
message(STATUS "BUILD_INCLUDE_PATH: ${BUILD_INCLUDE_PATH}")
message(STATUS "BUILD_LIB_PATH: ${BUILD_LIB_PATH}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "#################################################")
message(STATUS "#################################################") 

add_executable(osuTo2007 main.cpp)
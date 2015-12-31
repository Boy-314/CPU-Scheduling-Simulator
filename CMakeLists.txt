cmake_minimum_required(VERSION 3.3)
project(Lab_2_Tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp cpuScheduler.cpp Process.h verboseMethods.h schedulerMethods.h)
add_executable(Lab_2_Tests ${SOURCE_FILES})
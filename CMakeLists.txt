cmake_minimum_required(VERSION 3.18.2...4.0)
project(ctock-seer LANGUAGES CXX)

message([STATUS] "Using GCC options \"-Wall -Wextra std=c++23 -fmodules -O3,\" change if necessary")

add_compile_options(-Wall -Wextra -std=c++23 -fmodules -O3)

message([STATUS] "Python version is set to EXACT (1.13), change if necessary")

find_package(Python3 3.13 EXACT COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

add_executable(ctock-seer src/ctock-seer.cpp)
target_link_libraries(ctock-seer PRIVATE pybind11::embed)
cmake_minimum_required(VERSION 3.23)
project(cpp_tut VERSION 1.0.0 LANGUAGES CXX)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cpp_tut
    src/main.cpp
)
cmake_minimum_required(VERSION 3.22)
project(ksh)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp src/syscall_cpp_wrapper.cpp src/utils.cpp)

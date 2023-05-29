cmake_minimum_required(VERSION 3.0.0)
project(client VERSION 0.1.0)

set(CMAKE_CXX_VERSION 20)

add_compile_definitions(___WINDOWS___)

include(CTest)
enable_testing()

add_executable(client main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

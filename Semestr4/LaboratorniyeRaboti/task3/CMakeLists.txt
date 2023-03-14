cmake_minimum_required(VERSION 3.25)
project(task3)

set(CMAKE_CXX_STANDARD 20)

add_executable(task3 main.cpp memory.h memory.cpp sorted_list_memory.h sorted_list_memory.cpp nlogger/cpp_time.cpp nlogger/logger_builder.cpp nlogger/logger.cpp nlogger/basics.cpp nlogger/standard_logger.cpp nlogger/standard_logger_builder.cpp)

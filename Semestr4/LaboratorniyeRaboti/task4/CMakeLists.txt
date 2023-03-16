cmake_minimum_required(VERSION 3.25)
project(task4)

set(CMAKE_CXX_STANDARD 20)

add_executable(task4 main.cpp memory_boundary_descriptors.cpp memory_boundary_descriptors.h memory.cpp memory.h nlogger/basics.cpp nlogger/basics.h nlogger/cpp_time.cpp nlogger/cpp_time.h nlogger/logger.cpp nlogger/logger.h nlogger/logger_builder.cpp nlogger/logger_builder.h nlogger/standard_logger.cpp nlogger/standard_logger.h nlogger/standard_logger_builder.cpp nlogger/standard_logger_builder.h nlogger/nlohmann/json.hpp sorted_list_memory.h sorted_list_memory.cpp)

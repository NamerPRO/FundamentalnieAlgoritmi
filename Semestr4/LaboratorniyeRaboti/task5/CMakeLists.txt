cmake_minimum_required(VERSION 3.25)
project(task5)

set(CMAKE_CXX_STANDARD 20)

add_executable(task5 main.cpp nlogger/basics.cpp nlogger/basics.h nlogger/cpp_time.cpp nlogger/cpp_time.h nlogger/logger.cpp nlogger/logger.h nlogger/logger_builder.cpp nlogger/logger_builder.h nlogger/standard_logger.cpp nlogger/standard_logger.h nlogger/standard_logger_builder.cpp nlogger/standard_logger_builder.h nlogger/nlohmann/json.hpp memory.cpp memory.h buddy_memory_allocator.cpp buddy_memory_allocator.h nlogger/logger_helper.cpp sorted_list_memory.h sorted_list_memory.cpp memory_boundary_descriptors.cpp allocate_type_helper.h)

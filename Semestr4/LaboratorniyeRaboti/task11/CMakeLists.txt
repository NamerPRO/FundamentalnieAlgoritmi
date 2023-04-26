cmake_minimum_required(VERSION 3.25)
project(task11)

set(CMAKE_CXX_STANDARD 20)

add_executable(task11 main.cpp binary_search_tree.cpp binary_search_tree.h associative_container.cpp associative_container.h nmalloc.cpp nmalloc.h nmemory/buddy_memory_allocator.cpp nmemory/buddy_memory_allocator.h nmemory/memory.cpp nmemory/memory_boundary_descriptors.cpp nmemory/memory_boundary_descriptors.h nmemory/sorted_list_memory.cpp nmemory/standard_allocator.cpp nmemory/standard_allocator.hpp nmemory/nlogger/basics.cpp nmemory/nlogger/cpp_time.cpp nmemory/nlogger/logger.cpp nmemory/nlogger/logger_builder.cpp nmemory/nlogger/logger_helper.cpp nmemory/nlogger/standard_logger.cpp nmemory/nlogger/standard_logger_builder.cpp)

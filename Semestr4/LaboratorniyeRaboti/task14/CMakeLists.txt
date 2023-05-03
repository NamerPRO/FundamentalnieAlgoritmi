cmake_minimum_required(VERSION 3.0.0)
project(task14 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(task14 main.cpp nbst/binary_search_tree.h nbst/associative_container.h nbst/nmalloc.cpp nbst/nmalloc.h nmemory/buddy_memory_allocator.cpp nmemory/buddy_memory_allocator.h nmemory/memory.cpp nmemory/memory_boundary_descriptors.cpp nmemory/memory_boundary_descriptors.h nmemory/sorted_list_memory.cpp nmemory/standard_allocator.cpp nmemory/standard_allocator.hpp nmemory/nlogger/basics.cpp nmemory/nlogger/cpp_time.cpp nmemory/nlogger/logger.cpp nmemory/nlogger/logger_builder.cpp nmemory/nlogger/logger_helper.cpp nmemory/nlogger/standard_logger.cpp nmemory/nlogger/standard_logger_builder.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

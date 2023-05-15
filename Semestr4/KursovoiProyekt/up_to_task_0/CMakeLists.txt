cmake_minimum_required(VERSION 3.0.0)
project(task VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(
    task
    main.cpp
    navl/avl_tree.h
    nbst/associative_container.h
    nbst/binary_search_tree.h
    nbst/nmalloc.cpp
    nbst/nmalloc.h
    nmemory/allocate_type_helper.h
    nmemory/buddy_memory_allocator.cpp
    nmemory/buddy_memory_allocator.h
    nmemory/memory_boundary_descriptors.cpp
    nmemory/memory_boundary_descriptors.h
    nmemory/memory.cpp
    nmemory/memory.h
    nmemory/sorted_list_memory.cpp
    nmemory/sorted_list_memory.h
    nmemory/standard_allocator.cpp
    nmemory/standard_allocator.hpp
    nmemory/nlogger/basics.cpp
    nmemory/nlogger/basics.h
    nmemory/nlogger/cpp_time.cpp
    nmemory/nlogger/cpp_time.h
    nmemory/nlogger/logger_builder.cpp
    nmemory/nlogger/logger_builder.h
    nmemory/nlogger/logger_helper.cpp
    nmemory/nlogger/logger_helper.h
    nmemory/nlogger/logger.cpp
    nmemory/nlogger/logger.h
    nmemory/nlogger/standard_logger_builder.cpp
    nmemory/nlogger/standard_logger_builder.h
    nmemory/nlogger/standard_logger.cpp
    nmemory/nlogger/standard_logger.h
    nmemory/nlogger/nlohmann/json.hpp

    pipeline_manager/pipeline_comporators.cpp
    pipeline_manager/pipeline_interpriter.cpp
    pipeline_manager/pipeline.h
    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

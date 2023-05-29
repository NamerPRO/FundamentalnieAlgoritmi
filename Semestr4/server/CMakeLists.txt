cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(
    server

    flyweight_string/string_manager.cpp
    flyweight_string/string_manager.h

    ipc_manager/semaphores.cpp
    ipc_manager/semaphores.h
    ipc_manager/shared_memory.cpp
    ipc_manager/shared_memory.h

    iterators_manager/iterators.h

    navl/avl_tree.h

    nbst/associative_container.h
    nbst/binary_search_tree.h
    nbst/nmalloc.cpp
    nbst/nmalloc.h

    nbtree/btree.h

    nmemory/nlogger/nlohmann/json.hpp
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

    nsplay/splay_tree.h

    pipeline_commands_manager/base_command.h
    pipeline_commands_manager/command_invoker.h
    pipeline_commands_manager/commands_creator.h
    pipeline_commands_manager/pipeline_commands.cpp
    pipeline_commands_manager/pipeline_commands.h

    pipeline_manager/pipeline_base.h
    pipeline_manager/pipeline_comporators.cpp
    pipeline_manager/pipeline_data_save.h
    pipeline_manager/pipeline.h

    pipeline_transfer/pipeline_deserializer.cpp
    pipeline_transfer/pipeline_deserializer.h
    pipeline_transfer/pipeline_serializer.cpp
    pipeline_transfer/pipeline_serializer.h

    storage_manager/data_storage.cpp
    storage_manager/data_storage.h

    transfer_manager/standard_receiver.h
    transfer_manager/standard_sender.h
    
    main.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

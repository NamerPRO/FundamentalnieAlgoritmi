#ifndef TASK3_SORTED_LIST_MEMORY_H
#define TASK3_SORTED_LIST_MEMORY_H

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <stdexcept>

#include "nlogger/standard_logger.h"
#include "nlogger/standard_logger_builder.h"
#include "memory.h"

#define _RESERVED_SERVICE_MEMORY_SORTED (sizeof(intptr_t) + sizeof(uint32_t))

namespace nmemory_standards {

    class sorted_list_memory : public nmemory::memory {

    private:

        void const * _memory;

        bool const _was_allocated_via_constructor;

        const size_t _memory_size;

        const nmemory::memory::allocate_type _alloc_type;

        nlogger::logger * _memory_logger;

        void * first_fit_allocator(size_t target_size) const;

        void * best_fit_allocator(size_t target_size) const;

        void * worst_fit_allocator(size_t target_size) const;

        static void * get_list_item_next_data(void * node_ptr);

//        void * set_service_block_to_null(char * ptr) const;

        enum class service_block_type {

            empty,
            reserved

        };

        static void * set_service_block_data(void * to_service_data_ptr, service_block_type type, uint32_t size, ...);

        static std::pair<uint32_t, void *> get_service_block_data(void * to_service_data_ptr, service_block_type type);

//        bool has_space_for_independent_block_after_allocation(void * to_empty_memory_block_ptr, uint32_t size_to_allocate) const;

        static uint32_t get_balanced_allocation_space(void * to_empty_memory_block_ptr, uint32_t size_to_allocate);

        void * get_memory_heap_start() const;

//        intptr_t get_pointer_distance(void * ptr_1, void * ptr_2) const;

        void log_memory(std::string const & message, nlogger::logger::severity level) const;

        static void * reserve_block(void * prev_node_ptr, void * cur_node_ptr, size_t target_size, std::pair<uint32_t, void *> cur_empty_block_data);

    public:

        sorted_list_memory(void const * memory, size_t memory_size, nmemory::memory::allocate_type type, nlogger::logger * memory_logger);

//        sorted_list_memory(void const * memory, size_t memory_size, allocate_type type);

        sorted_list_memory(sorted_list_memory const &) = delete;

        sorted_list_memory & operator=(sorted_list_memory const &) = delete;

        void * allocate(size_t target_size) const override;

        void deallocate(void const * target_to_deallocate) const override;

        ~sorted_list_memory() override;
    };

}

#endif //TASK3_SORTED_LIST_MEMORY_H

#ifndef TASK4_MEMORY_BOUNDARY_DESCRIPTORS_H
#define TASK4_MEMORY_BOUNDARY_DESCRIPTORS_H

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdint>

#include "memory.h"
#include "nlogger/logger_helper.h"
#include "nlogger/standard_logger.h"
#include "nlogger/standard_logger_builder.h"
#include "allocate_type_helper.h"

#define _RESERVED_SERVICE_MEMORY (sizeof(intptr_t) * 2 + sizeof(uint32_t))

namespace nmemory_standards {

class boundary_descriptors_memory final : public nmemory::memory,
        protected logger_help,
        public allocate_type_helper {

    private:

        void const * _memory;

        const size_t _memory_size;

        const nmemory_standards::allocate_type_helper::allocate_type _alloc_type;

        nmemory::memory * _allocator;

        void * first_fit_allocator(size_t target_size) const;

        void * best_fit_allocator(size_t target_size) const;

        void * worst_fit_allocator(size_t target_size) const;

        static void * get_next_block_ptr(void * to_service_data_ptr);

        static void * get_past_block_ptr(void * to_service_data_ptr);

        static void * set_service_block_data(void * to_service_data_ptr, uint32_t target_size, void const * past_block_ptr, void const * next_block_ptr);

        void * get_memory_heap_start() const;

//        static uint32_t get_balanced_block_size(uint32_t target_size, uint32_t empty_block_size);

        struct service_block_data {

            void * past_block_ptr;
            void * next_block_ptr;
            uint32_t target_size;

        };

        enum class service_block_type {

            not_last,
            last

        };

        void * reserve_block(service_block_type block_type, service_block_data const & current_block_data, uint32_t const target_size, void * current_block_ptr) const;

        static service_block_data get_service_block_data(void * to_service_data_ptr);

        static uint32_t get_service_empty_block_size(void * border_ptr_past, void * border_ptr_next);

    public:

        boundary_descriptors_memory(nmemory::memory * allocator, size_t memory_size, nmemory_standards::allocate_type_helper::allocate_type type, nlogger::logger * memory_logger);

        boundary_descriptors_memory(boundary_descriptors_memory const &) = delete;

        boundary_descriptors_memory & operator=(boundary_descriptors_memory const &) = delete;

        void * allocate(size_t target_size) const override;

        void deallocate(void const * target_to_deallocate) const override;

        ~boundary_descriptors_memory() override;

    };

}

#endif //TASK4_MEMORY_BOUNDARY_DESCRIPTORS_H

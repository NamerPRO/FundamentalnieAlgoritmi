#ifndef TASK5_BUDDY_MEMORY_ALLOCATOR_H
#define TASK5_BUDDY_MEMORY_ALLOCATOR_H

#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include <limits>

#include "memory.h"
#include "./nlogger/standard_logger.h"
#include "./nlogger/standard_logger_builder.h"
#include "./nlogger/logger_helper.h"

namespace nmemory_standards {

    class buddy_memory_allocator final : public nmemory::memory,
            public nmemory_standards::logger_help {

#define MINIMUM_BLOCK_POWER 5
#define MINIMUM_BLOCK_SIZE 32
#define RESERVED_BLOCK_SIZE (sizeof(uint32_t) + sizeof(char))

    private:

        void * _memory;

        nmemory::memory const * _allocator;

        const uint32_t _memory_size;

        bool const _was_allocator_passed_to_constructor;

        static void set_block_data(bool is_block_empty, void * to_block_ptr, uint32_t power_of_two, void * past_node_ptr, void * next_node_ptr);

        struct node_data {

            void * past_node;
            void * next_node;
            int64_t power_of_two;

        };

        void * get_buddy_ptr(void * current_node_ptr, uint32_t new_node_size) const;

        static bool is_block_empty(void * block_ptr);

        static node_data get_block_data(bool is_block_empty, void * to_block_ptr);

        void * get_memory_heap_start() const;

    public:

        explicit buddy_memory_allocator(nmemory::memory const * allocator, size_t memory_size, nlogger::logger const * memory_logger);

        buddy_memory_allocator(buddy_memory_allocator const &) = delete;

        buddy_memory_allocator & operator=(buddy_memory_allocator const &) = delete;

        void * allocate(size_t target_size) const override;

        void deallocate(void const * target_to_deallocate) const override;

        ~buddy_memory_allocator() override;

    };

}


#endif //TASK5_BUDDY_MEMORY_ALLOCATOR_H

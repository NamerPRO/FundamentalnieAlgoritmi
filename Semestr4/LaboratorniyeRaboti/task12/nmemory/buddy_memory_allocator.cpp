#include "buddy_memory_allocator.h"

nmemory_standards::buddy_memory_allocator::buddy_memory_allocator(
        nmemory::memory const * allocator,
        size_t memory_size,
        nlogger::logger const * memory_logger
        ) : logger_help(const_cast<nlogger::logger *>(memory_logger)),
            _allocator(allocator),
            _memory_size(memory_size),
            _was_allocator_passed_to_constructor(_allocator != nullptr) {

//#ifndef SUSPEND_IRRATIONAL_MEMORY_USAGE_DETECTION
//    auto possible_power_of_two = std::log2(memory_size);
//    double _;
//    double possible_float_part_of_power_of_two = std::modf(possible_power_of_two, &_);
//    if (std::abs(possible_float_part_of_power_of_two) >= std::numeric_limits<double>::epsilon()) {
//        throw std::runtime_error("Irrational memory usage detected! Allocated " + std::to_string(_memory_size) + " bytes, but only " + std::to_string(1 << static_cast<size_t>(possible_power_of_two)) + " bytes will be used! Define SUSPEND_IRRATIONAL_MEMORY_USAGE_DETECTION macros to ignore this.");
//    }
//#endif

    nmemory_standards::logger_help::log_memory("Creating allocator object.", nlogger::logger::severity::information);

    if (_memory_size < MINIMUM_BLOCK_SIZE) {
        throw std::runtime_error("Too few memory requested! Allocator does not have space to store its data.");
    }

    if (_allocator != nullptr) {
        _memory = _allocator->allocate(_memory_size + MINIMUM_BLOCK_SIZE);
    } else {
        _memory = ::operator new(_memory_size + MINIMUM_BLOCK_SIZE);
    }

    char * working_memory_ptr = reinterpret_cast<char *>(const_cast<void *>(_memory));
    set_block_data(true, working_memory_ptr, 0, nullptr, working_memory_ptr + MINIMUM_BLOCK_SIZE);
    working_memory_ptr += MINIMUM_BLOCK_SIZE;
    auto power_of_two = static_cast<uint32_t>(std::floor(std::log2(_memory_size)));
    set_block_data(true, working_memory_ptr, power_of_two, _memory, nullptr);

    nmemory_standards::logger_help::log_memory("Done! Allocator object successfully created.", nlogger::logger::severity::information);
}

void nmemory_standards::buddy_memory_allocator::deallocate(void const * target_to_deallocate) const {
    nmemory_standards::logger_help::log_memory("Deallocating memory at address: " + std::to_string(reinterpret_cast<intptr_t>(target_to_deallocate) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ".", nlogger::logger::severity::information);

    char * working_target_to_deallocate = reinterpret_cast<char *>(const_cast<void *>(target_to_deallocate)) - RESERVED_BLOCK_SIZE;

    node_data block_data = get_block_data(false, working_target_to_deallocate);

    auto * memory_block_ptr = reinterpret_cast<unsigned char *>(const_cast<void *>(target_to_deallocate));
    uint32_t target_size = (1 << block_data.power_of_two);
    std::string data_as_byte_collection;
    for (uint32_t i = 0; i < target_size; ++i) {
        data_as_byte_collection += std::to_string(static_cast<unsigned int>(*memory_block_ptr)) + ' ';
        ++memory_block_ptr;
    }
    log_memory("Target data as byte collection: " + data_as_byte_collection, nlogger::logger::severity::information);

    void * right_border_ptr = nullptr;
    void * merge_block_start = working_target_to_deallocate;
    bool is_block_right_neighbour_nullptr = false;
    auto maximum_power_of_two = static_cast<uint32_t>(std::floor(std::log2(_memory_size)));
    while (true) {
        void * buddy_ptr = get_buddy_ptr(merge_block_start, block_data.power_of_two);

        if (!is_block_empty(buddy_ptr)) {
            if (is_block_right_neighbour_nullptr) {
                right_border_ptr = nullptr;
            } else {
                right_border_ptr = std::max(buddy_ptr, reinterpret_cast<void *>(reinterpret_cast<char *>(merge_block_start) + (1 << block_data.power_of_two)));
            }
            break;
        }

        node_data buddy_data = get_block_data(true, buddy_ptr);

        if (buddy_data.power_of_two != block_data.power_of_two) {
            if (is_block_right_neighbour_nullptr || (buddy_data.next_node == nullptr && buddy_ptr < merge_block_start)) {
                right_border_ptr = nullptr;
            } else {
                right_border_ptr = std::max(buddy_ptr, reinterpret_cast<void *>(reinterpret_cast<char *>(merge_block_start) + (1 << block_data.power_of_two)));
            }
            break;
        }

        merge_block_start = std::min(reinterpret_cast<void *>(merge_block_start), buddy_ptr);

        ++block_data.power_of_two;

        if (buddy_data.next_node == nullptr) {
            is_block_right_neighbour_nullptr = true;
        }

        if (block_data.power_of_two == maximum_power_of_two) {
            right_border_ptr = nullptr;
            break;
        }

    }

    void * previous_list_node = get_memory_heap_start();
    void * current_list_node = get_block_data(true, previous_list_node).next_node; // get_list_node_data(previous_list_node).next_node;

    while (current_list_node < merge_block_start) {
        previous_list_node = current_list_node;
        current_list_node = get_block_data(true, previous_list_node).next_node;
        if (current_list_node == nullptr) {
            right_border_ptr = nullptr;
            break;
        }
    }

    node_data previous_node_data = get_block_data(true, previous_list_node);
    if (right_border_ptr == nullptr) {
        set_block_data(true, merge_block_start, block_data.power_of_two, previous_list_node, nullptr);
        set_block_data(true, previous_list_node, previous_node_data.power_of_two, previous_node_data.past_node, merge_block_start);
        return;
    }

    while (current_list_node < right_border_ptr) {
        current_list_node = get_block_data(true, current_list_node).next_node;
    }

    node_data current_node_data = get_block_data(true, current_list_node);
    set_block_data(true, merge_block_start, block_data.power_of_two, previous_list_node, current_list_node);
    set_block_data(true, previous_list_node, previous_node_data.power_of_two, previous_node_data.past_node, merge_block_start);
    set_block_data(true, current_list_node, current_node_data.power_of_two, merge_block_start, current_node_data.next_node);
}

void * nmemory_standards::buddy_memory_allocator::allocate(
        size_t target_size
        ) const {
    auto power_of_two = static_cast<uint32_t>(std::ceil(std::log2(static_cast<double>(target_size + RESERVED_BLOCK_SIZE))));

    nmemory_standards::logger_help::log_memory("Allocating " + std::to_string(1 << ((power_of_two < MINIMUM_BLOCK_POWER) ? MINIMUM_BLOCK_POWER : power_of_two)) + " bytes of memory. Really requested: " + std::to_string(target_size) + " bytes of memory.", nlogger::logger::severity::information);

    void * previous_list_node = get_memory_heap_start();
    void * current_list_node = get_block_data(true, previous_list_node).next_node;

    while (current_list_node != nullptr) {
        node_data current_node_data = get_block_data(true, current_list_node);
        if (power_of_two <= current_node_data.power_of_two) {
            void * previous_buddy_ptr = current_node_data.next_node;
            void * buddy_ptr = current_node_data.next_node;
            while (current_node_data.power_of_two != MINIMUM_BLOCK_POWER && power_of_two != current_node_data.power_of_two) {
                --current_node_data.power_of_two;
                buddy_ptr = get_buddy_ptr(current_list_node, current_node_data.power_of_two);
                if (previous_buddy_ptr != nullptr) {
                    node_data previous_buddy_data = get_block_data(true, previous_buddy_ptr);
                    set_block_data(true, previous_buddy_ptr, previous_buddy_data.power_of_two, buddy_ptr, previous_buddy_data.next_node);
                }
                set_block_data(true, current_list_node, current_node_data.power_of_two, previous_list_node, buddy_ptr);
                set_block_data(true, buddy_ptr, current_node_data.power_of_two, current_list_node, previous_buddy_ptr);
                previous_buddy_ptr = buddy_ptr;
            }
            node_data previous_list_node_data = get_block_data(true, previous_list_node);
            set_block_data(true, previous_list_node, previous_list_node_data.power_of_two, previous_list_node_data.past_node, buddy_ptr);
            if (buddy_ptr != nullptr) {
                node_data buddy_ptr_data = get_block_data(true, buddy_ptr);
                set_block_data(true, buddy_ptr, buddy_ptr_data.power_of_two, previous_list_node, buddy_ptr_data.next_node);
            }
            set_block_data(false, current_list_node, current_node_data.power_of_two, nullptr, nullptr);
            void * memory_address = reinterpret_cast<void *>(reinterpret_cast<char *>(current_list_node) + RESERVED_BLOCK_SIZE);
            nmemory_standards::logger_help::log_memory("Successfully allocated " + std::to_string(1 << current_node_data.power_of_two) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
            return memory_address;
        }
        previous_list_node = current_list_node;
        current_list_node = current_node_data.next_node;
    }

    throw std::runtime_error("Memory allocate exception!");
}

void nmemory_standards::buddy_memory_allocator::set_block_data(
        bool is_block_empty,
        void * to_block_ptr,
        uint32_t power_of_two,
        void * past_node_ptr,
        void * next_node_ptr
        ) {
    char * working_to_block_ptr = reinterpret_cast<char *>(to_block_ptr);

    *working_to_block_ptr = is_block_empty ? '1' : '0';
    ++working_to_block_ptr;

    std::memcpy(working_to_block_ptr, &power_of_two, sizeof(uint32_t));
    working_to_block_ptr += sizeof(uint32_t);

    if (is_block_empty) {
        auto null_memory = [](char * ptr) -> char * {
            for (int i = 0; i < sizeof(void *); ++i) {
                *ptr = 0;
                ++ptr;
            }
            return ptr;
        };

        if (past_node_ptr != nullptr) {
            std::memcpy(working_to_block_ptr, &past_node_ptr, sizeof(void *));
            working_to_block_ptr += sizeof(void *);
        } else {
            working_to_block_ptr = null_memory(working_to_block_ptr);
        }

        if (next_node_ptr != nullptr) {
            std::memcpy(working_to_block_ptr, &next_node_ptr, sizeof(void *));
        } else {
            null_memory(working_to_block_ptr);
        }
    }
}

nmemory_standards::buddy_memory_allocator::node_data nmemory_standards::buddy_memory_allocator::get_block_data(
        bool is_block_empty,
        void * to_block_ptr
        ) {
    char * working_to_block_ptr = reinterpret_cast<char *>(to_block_ptr) + 1;

    uint32_t block_size = 0;
    std::memcpy(&block_size, working_to_block_ptr, sizeof(uint32_t));
    working_to_block_ptr += sizeof(uint32_t);

    void * past_node_ptr = nullptr;
    void * next_node_ptr = nullptr;

    if (is_block_empty) {
        std::memcpy(&past_node_ptr, working_to_block_ptr, sizeof(void *));
        working_to_block_ptr += sizeof(void *);
        std::memcpy(&next_node_ptr, working_to_block_ptr, sizeof(void *));
    }

    return {past_node_ptr, next_node_ptr, block_size};
}

void * nmemory_standards::buddy_memory_allocator::get_memory_heap_start() const {
    return const_cast<void *>(_memory);
}

void * nmemory_standards::buddy_memory_allocator::get_buddy_ptr(void * current_node_ptr, uint32_t new_node_size) const {
    return reinterpret_cast<void *>(((reinterpret_cast<intptr_t>(current_node_ptr) - reinterpret_cast<intptr_t>(get_memory_heap_start()) - MINIMUM_BLOCK_SIZE) ^ (1 << new_node_size)) + reinterpret_cast<intptr_t>(get_memory_heap_start()) + MINIMUM_BLOCK_SIZE);
}

nmemory_standards::buddy_memory_allocator::~buddy_memory_allocator() {
    if (_was_allocator_passed_to_constructor) {
        _allocator->deallocate(_memory);
    } else {
        ::operator delete(_memory);
    }
}

bool nmemory_standards::buddy_memory_allocator::is_block_empty(void * block_ptr) {
    char is_block_empty_as_num;
    std::memcpy(&is_block_empty_as_num, block_ptr, sizeof(char));
    return is_block_empty_as_num == '1';
}

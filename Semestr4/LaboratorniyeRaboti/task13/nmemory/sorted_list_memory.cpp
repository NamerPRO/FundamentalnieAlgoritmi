#include "sorted_list_memory.h"

// fun minimum_memory_block_size() => return 2 * _RESERVED_SERVICE_MEMORY_SORTED_FOR_EMPTY_BLOCK

nmemory_standards::sorted_list_memory::~sorted_list_memory() noexcept {
    if (_allocator == nullptr) {
        ::operator delete(reinterpret_cast<char *>(const_cast<void *>(_memory)));
    } else {
        _allocator->deallocate(_memory);
    }
}

//nmemory_standards::sorted_list_memory::sorted_list_memory(const void * memory, size_t memory_size, nmemory_standards::sorted_list_memory::allocate_type type) : sorted_list_memory(memory, memory_size, type, nullptr) {}

nmemory_standards::sorted_list_memory::sorted_list_memory(nmemory::memory * allocator, size_t memory_size, nmemory_standards::allocate_type_helper::allocate_type type, nlogger::logger * memory_logger): logger_help(memory_logger), _memory_size(memory_size), _alloc_type(type) {
    if (_memory_size < 2 * _RESERVED_SERVICE_MEMORY_SORTED) {
        throw std::runtime_error("Not enough memory to store service information! Consider passing bigger memory block to constructor.");
    }

    _allocator = allocator;

    if (allocator != nullptr) {
        _memory = _allocator->allocate(_memory_size);
    } else {
        _memory = ::operator new(_memory_size);
    }

    char * working_section_ptr = const_cast<char *>(reinterpret_cast<char const *>(_memory) + _RESERVED_SERVICE_MEMORY_SORTED);

    set_service_block_data(const_cast<void *>(_memory), service_block_type::empty, 0, working_section_ptr); // \/ 2 * _RESERVED_SERVICE_MEMORY_SORTED_FOR_EMPTY_BLOCK + sizeof(void *)
    set_service_block_data(working_section_ptr, service_block_type::empty, _memory_size - _RESERVED_SERVICE_MEMORY_SORTED, nullptr);
}

uint32_t nmemory_standards::sorted_list_memory::get_balanced_allocation_space(void * to_empty_memory_block_ptr, uint32_t size_to_allocate) {
    std::pair<uint32_t, void *> empty_block_data = get_service_block_data(to_empty_memory_block_ptr, service_block_type::empty);
    uint32_t memory_delta = empty_block_data.first - size_to_allocate - _RESERVED_SERVICE_MEMORY_SORTED;
    if (memory_delta < _RESERVED_SERVICE_MEMORY_SORTED) {
        return empty_block_data.first - _RESERVED_SERVICE_MEMORY_SORTED;
    }
    return size_to_allocate;
}

std::pair<uint32_t, void *> nmemory_standards::sorted_list_memory::get_service_block_data(void * to_service_data_ptr, service_block_type type) {
    uint32_t block_size;
    std::memcpy(&block_size, to_service_data_ptr, sizeof(block_size));

    void * next_node_ptr = nullptr;
    if (type == service_block_type::empty) {
        std::memcpy(&next_node_ptr, reinterpret_cast<char *>(to_service_data_ptr) + sizeof(block_size), sizeof(void *));
    }

    return {block_size, next_node_ptr};
}

void * nmemory_standards::sorted_list_memory::set_service_block_data(void * to_service_data_ptr, service_block_type type, uint32_t size, ...) {
    void * node_next_ptr = nullptr;
    char * after_service_data_ptr = reinterpret_cast<char *>(to_service_data_ptr);

    std::memcpy(after_service_data_ptr, &size, sizeof(size));
    after_service_data_ptr += sizeof(size);

    if (type == service_block_type::empty) {
        va_list node_next_ptr_arg;
        va_start(node_next_ptr_arg, size);
        node_next_ptr = va_arg(node_next_ptr_arg, void *);
        va_end(node_next_ptr_arg);

        if (node_next_ptr != nullptr) {
            std::memcpy(after_service_data_ptr, &node_next_ptr, sizeof(node_next_ptr));
            after_service_data_ptr += sizeof(node_next_ptr);
        } else {
            for (intptr_t i = 0; i < sizeof(intptr_t); ++i) {
                *after_service_data_ptr = 0;
                ++after_service_data_ptr;
            }
        }
    } else {
        after_service_data_ptr += sizeof(node_next_ptr);
    }

    return after_service_data_ptr;
}

void * nmemory_standards::sorted_list_memory::get_list_item_next_data(void * node_ptr) {
    if (node_ptr == nullptr) {
        return nullptr;
    }
    return get_service_block_data(node_ptr, service_block_type::empty).second;
}

void * nmemory_standards::sorted_list_memory::get_memory_heap_start() const {
    return const_cast<void *>(_memory);
}

void * nmemory_standards::sorted_list_memory::reserve_block(void * prev_node_ptr, void * cur_node_ptr, size_t target_size, std::pair<uint32_t, void *> cur_empty_block_data) {
    uint32_t balanced_target_size = get_balanced_allocation_space(cur_node_ptr, target_size);
    void * to_allocated_memory_ptr;
    if (balanced_target_size + _RESERVED_SERVICE_MEMORY_SORTED == cur_empty_block_data.first) { // <=
        to_allocated_memory_ptr = set_service_block_data(cur_node_ptr, service_block_type::reserved, balanced_target_size + _RESERVED_SERVICE_MEMORY_SORTED);
        set_service_block_data(prev_node_ptr, service_block_type::empty, get_service_block_data(prev_node_ptr, service_block_type::empty).first, cur_empty_block_data.second);
    } else {
        to_allocated_memory_ptr = set_service_block_data(cur_node_ptr, service_block_type::reserved, balanced_target_size + _RESERVED_SERVICE_MEMORY_SORTED);
        void * new_node_ptr = reinterpret_cast<char *>(to_allocated_memory_ptr) + balanced_target_size;
        set_service_block_data(prev_node_ptr, service_block_type::empty, get_service_block_data(prev_node_ptr, service_block_type::empty).first, new_node_ptr);
        set_service_block_data(new_node_ptr, service_block_type::empty, cur_empty_block_data.first - balanced_target_size - _RESERVED_SERVICE_MEMORY_SORTED, cur_empty_block_data.second); // cur_empty_block_data.first - balanced_target_size - 2 * _RESERVED_SERVICE_MEMORY_SORTED_FOR_EMPTY_BLOCK
    }
    return to_allocated_memory_ptr;
}

void * nmemory_standards::sorted_list_memory::first_fit_allocator(size_t target_size) const {
    log_memory("Allocating " + std::to_string(target_size) + " bytes of memory via first-fit method.", nlogger::logger::severity::information);

    void * prev_node_ptr = get_memory_heap_start();
    void * cur_node_ptr = get_list_item_next_data(prev_node_ptr);

    if (cur_node_ptr == nullptr) {
        throw std::runtime_error("Memory allocate exception!");
    }

    std::pair<uint32_t, void *> cur_empty_block_data;
    do {
        cur_empty_block_data = get_service_block_data(cur_node_ptr, service_block_type::empty);
        if (target_size + _RESERVED_SERVICE_MEMORY_SORTED <= cur_empty_block_data.first) {
            void * memory_address = reserve_block(prev_node_ptr, cur_node_ptr, target_size, cur_empty_block_data);
            log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
            return memory_address;
        }
        prev_node_ptr = cur_node_ptr;
        cur_node_ptr = get_list_item_next_data(prev_node_ptr);
    } while (cur_node_ptr != nullptr);

    throw std::runtime_error("Memory allocate exception!");
}

void * nmemory_standards::sorted_list_memory::best_fit_allocator(size_t target_size) const {
    log_memory("Allocating " + std::to_string(target_size) + " bytes of memory via best-fit method.", nlogger::logger::severity::information);

    void * prev_node_ptr = get_memory_heap_start();
    void * cur_node_ptr = get_list_item_next_data(prev_node_ptr);

    if (cur_node_ptr == nullptr) {
        throw std::runtime_error("Memory allocate exception!");
    }

    void * prev_node_ptr_best = nullptr;
    void * cur_node_ptr_best = nullptr;
    std::pair<uint32_t, void *> cur_empty_block_data_best;

    std::pair<uint32_t, void *> cur_empty_block_data;
    uint32_t best_size = _memory_size + 1;

    do {
        cur_empty_block_data = get_service_block_data(cur_node_ptr, service_block_type::empty);
        if (target_size + _RESERVED_SERVICE_MEMORY_SORTED <= cur_empty_block_data.first) {
            if (best_size >= cur_empty_block_data.first) {
                best_size = cur_empty_block_data.first;
                prev_node_ptr_best = prev_node_ptr;
                cur_node_ptr_best = cur_node_ptr;
                cur_empty_block_data_best = cur_empty_block_data;
            }
        }
        prev_node_ptr = cur_node_ptr;
        cur_node_ptr = get_list_item_next_data(prev_node_ptr);
    } while (cur_node_ptr != nullptr);

    if (prev_node_ptr_best == nullptr) {
        throw std::runtime_error("Memory allocate exception!");
    }

    void * memory_address = reserve_block(prev_node_ptr_best, cur_node_ptr_best, target_size, cur_empty_block_data_best);
    log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
    return memory_address;
}

void * nmemory_standards::sorted_list_memory::worst_fit_allocator(size_t target_size) const {
    log_memory("Allocating " + std::to_string(target_size) + " bytes of memory via worst-fit method.", nlogger::logger::severity::information);

    void * prev_node_ptr = get_memory_heap_start();
    void * cur_node_ptr = get_list_item_next_data(prev_node_ptr);

    if (cur_node_ptr == nullptr) {
        throw std::runtime_error("Memory allocate exception!");
    }

    void * prev_node_ptr_worst = nullptr;
    void * cur_node_ptr_worst = nullptr;
    std::pair<uint32_t, void *> cur_empty_block_data_worst;

    std::pair<uint32_t, void *> cur_empty_block_data;
    uint32_t worst_size = 0;

    do {
        cur_empty_block_data = get_service_block_data(cur_node_ptr, service_block_type::empty);
        if (target_size + _RESERVED_SERVICE_MEMORY_SORTED <= cur_empty_block_data.first) {
            if (worst_size <= cur_empty_block_data.first) {
                worst_size = cur_empty_block_data.first;
                prev_node_ptr_worst = prev_node_ptr;
                cur_node_ptr_worst = cur_node_ptr;
                cur_empty_block_data_worst = cur_empty_block_data;
            }
        }
        prev_node_ptr = cur_node_ptr;
        cur_node_ptr = get_list_item_next_data(prev_node_ptr);
    } while (cur_node_ptr != nullptr);

    if (prev_node_ptr_worst == nullptr) {
        throw std::runtime_error("Memory allocate exception!");
    }

    void * memory_address = reserve_block(prev_node_ptr_worst, cur_node_ptr_worst, target_size, cur_empty_block_data_worst);
    log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
    return memory_address;
}

void * nmemory_standards::sorted_list_memory::allocate(size_t target_size) const {
    switch (_alloc_type) {
        case nmemory_standards::allocate_type_helper::allocate_type::first_fit:
            return this->first_fit_allocator(target_size);
        case nmemory_standards::allocate_type_helper::allocate_type::best_fit:
            return this->best_fit_allocator(target_size);
        case nmemory_standards::allocate_type_helper::allocate_type::worst_fit:
            return this->worst_fit_allocator(target_size);
    }
    return nullptr;
}

void nmemory_standards::sorted_list_memory::deallocate(const void * target_to_deallocate) const {
    log_memory("Deallocating memory at address: " + std::to_string(reinterpret_cast<intptr_t>(target_to_deallocate) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ".", nlogger::logger::severity::information);

    char * moving_target_to_deallocate = reinterpret_cast<char *>(const_cast<void *>(target_to_deallocate)) - _RESERVED_SERVICE_MEMORY_SORTED;
    void * working_target_to_deallocate = moving_target_to_deallocate;

    if (has_logger()) {
        std::string data_as_byte_collection;
        uint32_t target_size = get_service_block_data(working_target_to_deallocate, service_block_type::reserved).first - _RESERVED_SERVICE_MEMORY_SORTED;
        auto * memory_block_ptr = reinterpret_cast<unsigned char *>(const_cast<void *>(target_to_deallocate));
        for (uint32_t i = 0; i < target_size; ++i) {
            data_as_byte_collection += std::to_string(static_cast<int>(*memory_block_ptr)) + ' ';
            ++memory_block_ptr;
        }
        log_memory("Target data as byte collection: " + data_as_byte_collection, nlogger::logger::severity::information);
    }

    void * prev_node_ptr = get_memory_heap_start();
    void * cur_node_ptr = get_list_item_next_data(prev_node_ptr);

    auto is_not_between = [](void * working_target_to_deallocate, void * prev_node_ptr, void * cur_node_ptr) -> bool {
        auto prev_node_ptr_as_int = reinterpret_cast<intptr_t>(prev_node_ptr);
        auto cur_node_ptr_as_int = reinterpret_cast<intptr_t>(cur_node_ptr);
        auto target_to_deallocate_as_int = reinterpret_cast<intptr_t>(working_target_to_deallocate);

        // !(prev_node_ptr_as_int < target_to_deallocate_as_int && target_to_deallocate_as_int < cur_node_ptr_as_int)
        if (cur_node_ptr == nullptr) {
            return false;
        } else {
            return prev_node_ptr_as_int >= target_to_deallocate_as_int || target_to_deallocate_as_int >= cur_node_ptr_as_int;
        }
    };

    while (is_not_between(working_target_to_deallocate, prev_node_ptr, cur_node_ptr)) {
        prev_node_ptr = cur_node_ptr;
        cur_node_ptr = get_list_item_next_data(prev_node_ptr);
    }

    std::pair<uint32_t, void *> target_to_deallocate_info = get_service_block_data(working_target_to_deallocate, service_block_type::reserved);
    std::pair<uint32_t, void *> prev_node_ptr_info = get_service_block_data(prev_node_ptr, service_block_type::empty);

    set_service_block_data(working_target_to_deallocate, service_block_type::empty, target_to_deallocate_info.first, cur_node_ptr);
    set_service_block_data(prev_node_ptr, service_block_type::empty, prev_node_ptr_info.first, working_target_to_deallocate);

    // При изменении служебной области в prev_node_ptr размер был сохранен => можно переиспользовать prev_node_ptr_info.first
    bool has_left_empty_block = false;
    if (moving_target_to_deallocate - prev_node_ptr_info.first == prev_node_ptr) {
        has_left_empty_block = true;
        // В target_to_deallocate_info.second лежит nullptr! Поэтому используем cur_node_ptr
        set_service_block_data(prev_node_ptr, service_block_type::empty, target_to_deallocate_info.first + prev_node_ptr_info.first, cur_node_ptr);
    }

    if (moving_target_to_deallocate + target_to_deallocate_info.first == cur_node_ptr) {
        std::pair<uint32_t, void *> cur_node_ptr_info = get_service_block_data(cur_node_ptr, service_block_type::empty);
        if (has_left_empty_block) {
            // Размер в служебной ячейке prev_node_ptr теперь обновлен, надо получить его заново
            set_service_block_data(prev_node_ptr, service_block_type::empty, get_service_block_data(prev_node_ptr, service_block_type::empty).first + cur_node_ptr_info.first, cur_node_ptr_info.second);
        } else {
            set_service_block_data(working_target_to_deallocate, service_block_type::empty,get_service_block_data(working_target_to_deallocate, service_block_type::empty).first + cur_node_ptr_info.first, cur_node_ptr_info.second);
        }
    }
}

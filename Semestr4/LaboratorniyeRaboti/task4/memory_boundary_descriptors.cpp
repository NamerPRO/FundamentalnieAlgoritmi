#include "memory_boundary_descriptors.h"

nmemory_standards::boundary_descriptors_memory::boundary_descriptors_memory(
        const void * memory,
        size_t memory_size,
        nmemory::memory::allocate_type type,
        nlogger::logger * memory_logger
        ) : _memory_size(memory_size), _was_allocated_via_constructor(memory == nullptr), _alloc_type(type) {

    if (_memory_size < _RESERVED_SERVICE_MEMORY) {
        throw std::runtime_error("Not enough memory to store service information! Consider passing bigger memory block to constructor.");
    }

    if (memory != nullptr) {
        _memory = memory;
    } else {
        _memory = new char[_memory_size];
    }

    _memory_logger = memory_logger;

    set_service_block_data(const_cast<void *>(_memory), _RESERVED_SERVICE_MEMORY, nullptr, nullptr);

}

void * nmemory_standards::boundary_descriptors_memory::allocate(
        size_t target_size
        ) const {
    switch (_alloc_type) {
        case nmemory::memory::allocate_type::first_fit:
            return this->first_fit_allocator(target_size);
        case nmemory::memory::allocate_type::best_fit:
            return this->best_fit_allocator(target_size);
        case nmemory::memory::allocate_type::worst_fit:
            return this->worst_fit_allocator(target_size);
    }
    return nullptr;
}

void * nmemory_standards::boundary_descriptors_memory::first_fit_allocator(
        size_t target_size
        ) const {
    log_memory("Allocating " + std::to_string(target_size) + " bytes of memory via first-fit method.", nlogger::logger::severity::information);

    void * current_block_ptr = get_next_block_ptr(const_cast<void *>(_memory));

    service_block_data current_block_data{};
    while (current_block_ptr != nullptr) {
        current_block_data = get_service_block_data(current_block_ptr);
        uint32_t empty_block_size = get_service_empty_block_size(current_block_data.past_block_ptr, current_block_ptr);

        if (target_size + _RESERVED_SERVICE_MEMORY <= empty_block_size) {
            void * memory_address = reserve_block(service_block_type::not_last, current_block_data, target_size, current_block_ptr);
            log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
            return memory_address;
        }

        if (current_block_data.next_block_ptr == nullptr) {
            break;
        }
        current_block_ptr = current_block_data.next_block_ptr;
    }

    if (current_block_ptr == nullptr) {
        current_block_ptr = this->get_memory_heap_start();
        current_block_data = get_service_block_data(current_block_ptr);
    }

    uint32_t empty_block_size = get_service_empty_block_size(current_block_ptr, reinterpret_cast<char *>(this->get_memory_heap_start()) + _memory_size);
    if (target_size + _RESERVED_SERVICE_MEMORY <= empty_block_size) {
        void * memory_address = reserve_block(service_block_type::last, current_block_data, target_size, current_block_ptr);
        log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
        return memory_address;
    }

    throw std::runtime_error("Memory allocate exception!");
}

void * nmemory_standards::boundary_descriptors_memory::best_fit_allocator(
        size_t target_size
        ) const {
    log_memory("Allocating " + std::to_string(target_size) + " bytes of memory via best-fit method.", nlogger::logger::severity::information);

    void * current_block_ptr = get_next_block_ptr(const_cast<void *>(_memory));

    uint32_t empty_block_size_best = _memory_size;
    service_block_data block_data_best{};
    void * block_ptr_best = nullptr;

    service_block_data current_block_data{};
    while (current_block_ptr != nullptr) {
        current_block_data = get_service_block_data(current_block_ptr);
        uint32_t empty_block_size = get_service_empty_block_size(current_block_data.past_block_ptr, current_block_ptr);

        if (target_size + _RESERVED_SERVICE_MEMORY <= empty_block_size) {
            if (empty_block_size_best >= empty_block_size) {
                empty_block_size_best = empty_block_size;
                block_data_best = current_block_data;
                block_ptr_best = current_block_ptr;
            }
        }

        if (current_block_data.next_block_ptr == nullptr) {
            break;
        }
        current_block_ptr = current_block_data.next_block_ptr;
    }

    if (current_block_ptr == nullptr) {
        current_block_ptr = this->get_memory_heap_start();
        current_block_data = get_service_block_data(current_block_ptr);
    }

    uint32_t empty_block_size = get_service_empty_block_size(current_block_ptr, reinterpret_cast<char *>(this->get_memory_heap_start()) + _memory_size);
    if (target_size + _RESERVED_SERVICE_MEMORY <= empty_block_size) {
        if (empty_block_size_best >= empty_block_size) {
            void * memory_address = reserve_block(service_block_type::last, current_block_data, target_size, current_block_ptr);
            log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
            return memory_address;
        }
    }

    if (empty_block_size_best == _memory_size) {
        throw std::runtime_error("Memory allocate exception!");
    }

    void * memory_address = reserve_block(service_block_type::not_last, block_data_best, target_size, block_ptr_best);
    log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
    return memory_address;
}

void * nmemory_standards::boundary_descriptors_memory::worst_fit_allocator(
        size_t target_size
        ) const {
    log_memory("Allocating " + std::to_string(target_size) + " bytes of memory via worst-fit method.", nlogger::logger::severity::information);

    void * current_block_ptr = get_next_block_ptr(const_cast<void *>(_memory));

    uint32_t empty_block_size_worst = 0;
    service_block_data block_data_worst{};
    void * block_ptr_worst = nullptr;

    service_block_data current_block_data{};
    while (current_block_ptr != nullptr) {
        current_block_data = get_service_block_data(current_block_ptr);
        uint32_t empty_block_size = get_service_empty_block_size(current_block_data.past_block_ptr, current_block_ptr);

        if (target_size + _RESERVED_SERVICE_MEMORY <= empty_block_size) {
            if (empty_block_size_worst <= empty_block_size) {
                empty_block_size_worst = empty_block_size;
                block_data_worst = current_block_data;
                block_ptr_worst = current_block_ptr;
            }
        }

        if (current_block_data.next_block_ptr == nullptr) {
            break;
        }
        current_block_ptr = current_block_data.next_block_ptr;
    }

    if (current_block_ptr == nullptr) {
        current_block_ptr = this->get_memory_heap_start();
        current_block_data = get_service_block_data(current_block_ptr);
    }

    uint32_t empty_block_size = get_service_empty_block_size(current_block_ptr, reinterpret_cast<char *>(this->get_memory_heap_start()) + _memory_size);
    if (target_size + _RESERVED_SERVICE_MEMORY <= empty_block_size) {
        if (empty_block_size_worst <= empty_block_size) {
            void * memory_address = reserve_block(service_block_type::last, current_block_data, target_size, current_block_ptr);
            log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
            return memory_address;
        }
    }

    if (empty_block_size_worst == 0) {
        throw std::runtime_error("Memory allocate exception!");
    }

    void * memory_address = reserve_block(service_block_type::not_last, block_data_worst, target_size, block_ptr_worst);
    log_memory("Successfully allocated " + std::to_string(target_size) + " bytes of memory (addr = " + std::to_string(reinterpret_cast<intptr_t>(memory_address) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ").", nlogger::logger::severity::information);
    return memory_address;
}

void * nmemory_standards::boundary_descriptors_memory::set_service_block_data(
        void * to_service_data_ptr,
        uint32_t target_size,
        void const * past_block_ptr,
        void const * next_block_ptr
        ) {

    auto set_to_nullptr = [](char * to_ptr) -> char * {
        for (intptr_t i = 0; i < sizeof(intptr_t); ++i) {
            *to_ptr = 0;
            ++to_ptr;
        }
        return to_ptr;
    };

    char * working_to_service_data_ptr = reinterpret_cast<char *>(to_service_data_ptr);

    if (past_block_ptr != nullptr) {
        std::memcpy(working_to_service_data_ptr, &past_block_ptr, sizeof(void *));
        working_to_service_data_ptr += sizeof(void *);
    } else {
        working_to_service_data_ptr = set_to_nullptr(working_to_service_data_ptr);
    }

    if (next_block_ptr != nullptr) {
        std::memcpy(working_to_service_data_ptr, &next_block_ptr, sizeof(void *));
        working_to_service_data_ptr += sizeof(void *);
    } else {
        working_to_service_data_ptr = set_to_nullptr(working_to_service_data_ptr);
    }

    std::memcpy(working_to_service_data_ptr, &target_size, sizeof(uint32_t));
    working_to_service_data_ptr += sizeof(uint32_t);

    return working_to_service_data_ptr;
}

nmemory_standards::boundary_descriptors_memory::service_block_data nmemory_standards::boundary_descriptors_memory::get_service_block_data(
        void * to_service_data_ptr
        ) {
    void * past_block_ptr;
    void * next_block_ptr;
    uint32_t target_size;

    char * working_to_service_data_ptr = reinterpret_cast<char *>(to_service_data_ptr);
    std::memcpy(&past_block_ptr, working_to_service_data_ptr, sizeof(void *));
    working_to_service_data_ptr += sizeof(void *);
    std::memcpy(&next_block_ptr, working_to_service_data_ptr, sizeof(void *));
    working_to_service_data_ptr += sizeof(void *);
    std::memcpy(&target_size, working_to_service_data_ptr, sizeof(uint32_t));

    return {past_block_ptr, next_block_ptr, target_size};
}

uint32_t nmemory_standards::boundary_descriptors_memory::get_service_empty_block_size(
        void * border_ptr_past,
        void * border_ptr_next
        ) {
    uint32_t distance = reinterpret_cast<intptr_t>(border_ptr_next) - reinterpret_cast<intptr_t>(reinterpret_cast<char *>(border_ptr_past) +
            get_service_block_data(border_ptr_past).target_size);
    return (distance > 0) ? distance : -distance;
}

void * nmemory_standards::boundary_descriptors_memory::get_next_block_ptr(
        void * to_service_data_ptr
        ) {
    return get_service_block_data(to_service_data_ptr).next_block_ptr;
}

void * nmemory_standards::boundary_descriptors_memory::get_past_block_ptr(
        void * to_service_data_ptr
        ) {
    return get_service_block_data(to_service_data_ptr).past_block_ptr;
}

void * nmemory_standards::boundary_descriptors_memory::get_memory_heap_start() const {
    return const_cast<void *>(_memory);
}

void * nmemory_standards::boundary_descriptors_memory::reserve_block(
        service_block_type block_type,
        service_block_data const & current_block_data,
        uint32_t const target_size,
        void * current_block_ptr
        ) const {
    void * to_allocated_memory_ptr;
    if (block_type == service_block_type::not_last) {
        service_block_data past_block_data = get_service_block_data(current_block_data.past_block_ptr);
        char * empty_block_ptr = reinterpret_cast<char *>(current_block_data.past_block_ptr) + past_block_data.target_size;
        to_allocated_memory_ptr = set_service_block_data(empty_block_ptr, target_size + _RESERVED_SERVICE_MEMORY, current_block_data.past_block_ptr, current_block_ptr);
        set_service_block_data(current_block_ptr, current_block_data.target_size, empty_block_ptr, current_block_data.next_block_ptr);
        set_service_block_data(current_block_data.past_block_ptr, past_block_data.target_size, past_block_data.past_block_ptr, empty_block_ptr);
    } else {
        char * empty_block_ptr = reinterpret_cast<char *>(current_block_ptr) + current_block_data.target_size;
        to_allocated_memory_ptr = set_service_block_data(empty_block_ptr, target_size + _RESERVED_SERVICE_MEMORY, current_block_ptr, nullptr);
        set_service_block_data(current_block_ptr, current_block_data.target_size, current_block_data.past_block_ptr, empty_block_ptr);
    }
    return to_allocated_memory_ptr;
}

void nmemory_standards::boundary_descriptors_memory::deallocate(
        void const * target_to_deallocate
        ) const {
    log_memory("Deallocating memory at address: " + std::to_string(reinterpret_cast<intptr_t>(target_to_deallocate) - reinterpret_cast<intptr_t>(get_memory_heap_start())) + ".", nlogger::logger::severity::information);

    void * working_target_to_deallocate = reinterpret_cast<char *>(const_cast<void *>(target_to_deallocate)) - _RESERVED_SERVICE_MEMORY;
    service_block_data working_target_to_deallocate_data = get_service_block_data(working_target_to_deallocate);

    auto * memory_block_ptr = reinterpret_cast<unsigned char*>(const_cast<void *>(target_to_deallocate));
    uint32_t target_size = working_target_to_deallocate_data.target_size - _RESERVED_SERVICE_MEMORY;
    std::string data_as_byte_collection;
    for (uint32_t i = 0; i < target_size; ++i) {
        data_as_byte_collection += std::to_string(static_cast<unsigned int>(*memory_block_ptr)) + ' ';
        ++memory_block_ptr;
    }
    log_memory("Target data as byte collection: " + data_as_byte_collection, nlogger::logger::severity::information);

    service_block_data past_block_data = get_service_block_data(working_target_to_deallocate_data.past_block_ptr);

    set_service_block_data(working_target_to_deallocate_data.past_block_ptr, past_block_data.target_size, past_block_data.past_block_ptr, working_target_to_deallocate_data.next_block_ptr);
    if (working_target_to_deallocate_data.next_block_ptr != nullptr) {
        service_block_data next_block_data = get_service_block_data(working_target_to_deallocate_data.next_block_ptr);
        set_service_block_data(working_target_to_deallocate_data.next_block_ptr, next_block_data.target_size, working_target_to_deallocate_data.past_block_ptr, next_block_data.next_block_ptr);
    }
}

nmemory_standards::boundary_descriptors_memory::~boundary_descriptors_memory() {
    if (_was_allocated_via_constructor) {
        delete[] reinterpret_cast<char *>(const_cast<void *>(_memory));
    }
}

void nmemory_standards::boundary_descriptors_memory::log_memory(std::string const & message, nlogger::logger::severity level) const {
    if (this->_memory_logger == nullptr) {
        return;
    }
    _memory_logger->log(message, level);
}

//uint32_t nmemory_standards::boundary_descriptors_memory::get_balanced_block_size(
//        uint32_t target_size,
//        uint32_t empty_block_size
//        ) {
//    if (empty_block_size - (target_size + _RESERVED_SERVICE_MEMORY) < _RESERVED_SERVICE_MEMORY) {
//        return empty_block_size - _RESERVED_SERVICE_MEMORY;
//    }
//    return target_size;
//}

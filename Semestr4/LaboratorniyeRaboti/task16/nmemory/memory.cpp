#include "memory.h"

nmemory::memory::~memory() = default;

void * nmemory::memory::operator+=(size_t target_size) const {
    return this->allocate(target_size);
}

void nmemory::memory::operator-=(void const * target_to_deallocate) const {
    this->deallocate(target_to_deallocate);
}
#include "memory_allocator.hpp"

nmemory::memory::~memory() {}

void nmemory::memory::operator += (std::pair<void**, int> target) {
  *target.first = this->allocate(target.second);
}

void nmemory::memory::operator -= (void const * target_to_deallocate) {
  this->deallocate(target_to_deallocate);
}

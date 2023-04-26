#include "nmalloc.h"

namespace ntools {

    void nmalloc::free(void *target_to_deallocate) {
        if (_allocator == nullptr) {
            ::operator delete(target_to_deallocate);
        } else {
            _allocator->deallocate(target_to_deallocate);
        }
    }

    nmalloc::nmalloc(nmemory::memory * allocator) : _allocator(allocator) {}

} // ntools
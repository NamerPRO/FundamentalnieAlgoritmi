#ifndef TASK11_NMALLOC_H
#define TASK11_NMALLOC_H

#include "../nmemory/memory.h"

namespace ntools {

    class nmalloc {

    protected:

        nmemory::memory * _allocator;

        template<typename T = void *>
        T malloc(
            size_t size_to_allocate
        ) {
            if (_allocator == nullptr) {
                return reinterpret_cast<T>(::operator new(size_to_allocate));
            }
            return reinterpret_cast<T>(_allocator->allocate(size_to_allocate));
        }

        void free(
            void * target_to_deallocate
        );

        explicit nmalloc(
            nmemory::memory * allocator
        );

    };

} // ntools

#endif //TASK11_NMALLOC_H

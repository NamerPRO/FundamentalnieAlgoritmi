#ifndef MEMORY
#define MEMORY

#include <iostream>

namespace nmemory {

    class memory {

    public:

        enum class allocate_type {

            first_fit,
            best_fit,
            worst_fit

        };

        virtual void * allocate(size_t target_size) const = 0;

        virtual void deallocate(void const * target_to_deallocate) const = 0;

        virtual ~memory();

        void * operator+=(size_t target_size) const;

        void operator-=(void const * target_to_deallocate) const;
    };

}

#endif
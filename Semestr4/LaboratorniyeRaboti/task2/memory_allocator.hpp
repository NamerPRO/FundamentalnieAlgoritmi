#ifndef ___MEMORY_ALLOCATOR___
#define ___MEMORY_ALLOCATOR___

#include <iostream>

namespace nmemory {

  class memory {

  public:

    void operator += (std::pair<void**, int> target);

    void operator -= (void const * const target_to_deallocate);

    virtual void* allocate(int target_size) const = 0;

    virtual void deallocate(void const * const target_to_deallocate) = 0;

    virtual ~memory();

  };

}

#endif

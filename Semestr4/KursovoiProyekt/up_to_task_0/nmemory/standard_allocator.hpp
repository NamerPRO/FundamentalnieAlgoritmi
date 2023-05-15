#ifndef ___STANDARD_ALLOCATOR___
#define ___STANDARD_ALLOCATOR___

#include <iostream>
#include <string>
#include <stdexcept>
#include <malloc.h>

#include "./nlogger/standard_logger.h"
#include "./nlogger/standard_logger_builder.h"
#include "memory.h"

namespace nstandards {

  class standard_allocator : public nmemory::memory {

  private:

    nlogger::logger* memory_logger;

  public:

    standard_allocator(standard_allocator const &) = delete;

    standard_allocator & operator= (standard_allocator const &) = delete;

    void* allocate(size_t target_size) const override;

    void deallocate(void const * target_to_deallocate) const override;

    explicit standard_allocator(nlogger::logger * mem_logger);

//    ~standard_allocator();

  };

}

#endif

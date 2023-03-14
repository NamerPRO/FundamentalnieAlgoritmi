#ifndef ___STANDARD_ALLOCATOR___
#define ___STANDARD_ALLOCATOR___

#include <iostream>
#include <string>
#include <stdexcept>
#include <malloc.h>

#include "./logger/standard_logger.hpp"
#include "./logger/standard_logger_builder.hpp"
#include "memory_allocator.hpp"

namespace nstandards {

  class standard_allocator : public nmemory::memory {

  private:

    nlogger::logger* memory_logger;

  public:

    standard_allocator(standard_allocator const &) = delete;

    standard_allocator & operator= (standard_allocator const &) = delete;

    void* allocate(int target_size) const override;

    void deallocate(void const * const target_to_deallocate) override;

    standard_allocator();

    ~standard_allocator();

  };

}

#endif

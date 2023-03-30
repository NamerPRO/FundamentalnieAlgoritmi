#include <iostream>

#include "buddy_memory_allocator.h"
#include "memory_boundary_descriptors.h"

#include "./nlogger/standard_logger.h"
#include "./nlogger/standard_logger_builder.h"

int main() {

#define MEM_SIZE (1 << 9)// (1 << 30)

    nmemory::memory * boundary_mem = new nmemory_standards::boundary_descriptors_memory(
            nullptr,
            MEM_SIZE + 100,
            nmemory_standards::allocate_type_helper::allocate_type::first_fit,
            nullptr
    );

    nlogger::logger_builder * builder = new standard_logger_builder();
    builder = builder->bind("logs.txt", nlogger::logger::severity::information);
    nlogger::logger * my_logger = builder->build();

    nmemory::memory * mem = new nmemory_standards::buddy_memory_allocator(
            boundary_mem,
            MEM_SIZE,
            my_logger
            );

    auto * var1 = reinterpret_cast<char *>(mem->allocate(sizeof(char)));
    *var1 = 'M';
    std::cout << *var1 << std::endl;

    auto * var2 = reinterpret_cast<int *>(mem->allocate(20 * sizeof(int)));
    for (int i = 0; i < 20; ++i) {
        var2[i] = i + 1;
    }
    for (int i = 0; i < 20; ++i) {
        std::cout << var2[i] << ' ';
    }
    std::cout << std::endl;

    auto * var3 = reinterpret_cast<char *>(mem->allocate(70 * sizeof(char)));
    std::strcpy(var3, "This is a super duper long string that I enjoy!");
    std::cout << var3 << std::endl;

    mem->deallocate(var1);
    mem->deallocate(var2);
    mem->deallocate(var3);

    auto * var11 = reinterpret_cast<char *>(mem->allocate(sizeof(char)));
    *var11 = 'M';
    std::cout << *var11 << std::endl;

    auto * var21 = reinterpret_cast<int *>(mem->allocate(20 * sizeof(int)));
    for (int i = 0; i < 20; ++i) {
        var21[i] = i + 1;
    }
    for (int i = 0; i < 20; ++i) {
        std::cout << var21[i] << ' ';
    }
    std::cout << std::endl;

    auto * var31 = reinterpret_cast<char *>(mem->allocate(70 * sizeof(char)));
    std::strcpy(var31, "This is a super duper long string that I enjoy!");
    std::cout << var31 << std::endl;

    mem->deallocate(var11);
    mem->deallocate(var21);
    mem->deallocate(var31);

    delete mem;
    delete boundary_mem;
    delete my_logger;
    delete builder;

    return 0;
}

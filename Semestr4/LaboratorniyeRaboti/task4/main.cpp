#include <iostream>

#include "./nlogger/standard_logger.h"
#include "./nlogger/standard_logger_builder.h"
#include "sorted_list_memory.h"
#include "memory_boundary_descriptors.h"

int main() {

#define MEM_SIZE_BASIC 10000
#define MEM_SIZE_TEST 5000

    nlogger::logger_builder * mem_builder = (new standard_logger_builder())
            ->bind("logs.txt", nlogger::logger::severity::information);
    nlogger::logger * mem_logger = mem_builder->build();

    nmemory::memory * my_sorted_memory = new nmemory_standards::sorted_list_memory(
            nullptr,
            MEM_SIZE_BASIC,
            nmemory::memory::allocate_type::first_fit,
            mem_logger
            );

    void * mem_for_allocator = my_sorted_memory->allocate(MEM_SIZE_TEST);

    nmemory::memory * my_memory = new nmemory_standards::boundary_descriptors_memory(
            mem_for_allocator,
            MEM_SIZE_TEST,
            nmemory::memory::allocate_type::first_fit,
            mem_logger
            );

    int * array_of_integers = reinterpret_cast<int *>(*my_memory += 10 * sizeof(int));

    for (int i = 0; i < 10; ++i) {
        array_of_integers[i] = i;
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << array_of_integers[i] << ' ';
    }
    std::cout << std::endl;

    // ===
    // int * var = my_memory->allocate<int *>(10 * sizeof(int))
    long long * my_number = reinterpret_cast<long long *>(my_memory->allocate(sizeof(long long)));
    *my_number = 123L;
    std::cout << *my_number << std::endl;

    *my_memory -= array_of_integers;

    char * i_like_minecraft = reinterpret_cast<char *>(my_memory->allocate(17 * sizeof(char)));
    strcpy(i_like_minecraft, "I like minecraft");
    std::cout << i_like_minecraft << std::endl;

    char * lol = reinterpret_cast<char *>(my_memory->allocate(3 * sizeof(char)));
    strcpy(lol, "go");
    std::cout << lol << std::endl;

    *my_memory -= lol;
    *my_memory -= i_like_minecraft;
    *my_memory -= my_number;

    delete my_memory;
    my_sorted_memory->deallocate(mem_for_allocator);
    delete my_sorted_memory;
    delete mem_logger;
    delete mem_builder;

    return 0;
}

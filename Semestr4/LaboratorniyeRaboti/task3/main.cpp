#include <iostream>
#include <stdexcept>
#include <cstring>

#include "nlogger/standard_logger.h"
#include "nlogger/standard_logger_builder.h"
#include "memory.h"
#include "sorted_list_memory.h"

#define MEM_SIZE 1000000

using cpp_memory = nmemory_standards::sorted_list_memory;

int main() {

    try {

        nlogger::logger_builder * mem_builder = new standard_logger_builder();
        nlogger::logger * mem_logger = mem_builder
                ->bind("logs.txt", nlogger::logger::severity::information)
                ->build();

        char * my_mem = new char[MEM_SIZE];
        nmemory::memory * allocator = new cpp_memory(my_mem,
                                                    MEM_SIZE,
                                                    cpp_memory ::allocate_type::first_fit,
                                                    mem_logger);

//        int * example = reinterpret_cast<int *>(*allocator += 10 * sizeof(int));
//        for (int i = 0; i < 10; ++i) {
//            example[i] = i;
//        }
//        for (int i = 0; i < 10; ++i) {
//            std::cout << example[i] << ' ';
//        }
//        std::cout << std::endl;
//
//        *allocator -= example;
//        delete mem_builder;
//        delete mem_logger;
//        delete allocator;
//        delete[] my_mem;
//        allocator->deallocate(example);

        auto * array_of_integers = reinterpret_cast<int *>(allocator->allocate(10 * sizeof(int)));

        for (int i = 0; i < 10; ++i) {
            array_of_integers[i] = i % 5 + 1;
        }

        for (int i = 0; i < 10; ++i) {
            std::cout << array_of_integers[i] << ' ';
        }
        std::cout << std::endl;

        auto * array_of_chars = reinterpret_cast<char *>(allocator->allocate(20 * sizeof(char)));
        std::strcpy(array_of_chars, "Hello World");
        std::cout << array_of_chars << std::endl;

        auto * long_number = reinterpret_cast<long long *>(allocator->allocate(sizeof(long long)));
        *long_number = 123L;
        std::cout << *long_number << std::endl;

        auto * integer_number2 = reinterpret_cast<int *>(allocator->allocate(sizeof(int)));
        *integer_number2 = 555;
        std::cout << *integer_number2 << std::endl;

        allocator->deallocate(array_of_chars);

        int * integer_number = reinterpret_cast<int *>(allocator->allocate(sizeof(int)));
        *integer_number = 456;
        std::cout << *integer_number << std::endl;

//        allocator->deallocate(integer_number2);

        auto * another_integer_number = reinterpret_cast<long long *>(allocator->allocate(sizeof(long long)));
        *another_integer_number = 777L;
        std::cout << *another_integer_number << std::endl;

        allocator->deallocate(array_of_integers);
        allocator->deallocate(long_number);
        allocator->deallocate(integer_number2);
        allocator->deallocate(integer_number);
        allocator->deallocate(another_integer_number);

         array_of_integers = reinterpret_cast<int *>(allocator->allocate(10 * sizeof(int)));

        for (int i = 0; i < 10; ++i) {
            array_of_integers[i] = i % 5 + 1;
        }

        for (int i = 0; i < 10; ++i) {
            std::cout << array_of_integers[i] << ' ';
        }
        std::cout << std::endl;

         array_of_chars = reinterpret_cast<char *>(allocator->allocate(20 * sizeof(char)));
        std::strcpy(array_of_chars, "Hello World");
        std::cout << array_of_chars << std::endl;

         long_number = reinterpret_cast<long long *>(allocator->allocate(sizeof(long long)));
        *long_number = 123L;
        std::cout << *long_number << std::endl;

         integer_number2 = reinterpret_cast<int *>(allocator->allocate(sizeof(int)));
        *integer_number2 = 555;
        std::cout << *integer_number2 << std::endl;

        allocator->deallocate(array_of_chars);

         integer_number = reinterpret_cast<int *>(allocator->allocate(sizeof(int)));
        *integer_number = 456;
        std::cout << *integer_number << std::endl;

//        allocator->deallocate(integer_number2);

         another_integer_number = reinterpret_cast<long long *>(allocator->allocate(sizeof(long long)));
        *another_integer_number = 777L;
        std::cout << *another_integer_number << std::endl;

        allocator->deallocate(array_of_integers);
        allocator->deallocate(long_number);
        allocator->deallocate(integer_number2);
        allocator->deallocate(integer_number);
        allocator->deallocate(another_integer_number);


        delete allocator;
        delete[] my_mem;

    } catch (std::runtime_error const & error) {
        std::cerr << "An error occurred: " << error.what() << std::endl;
    }

    return 0;
}

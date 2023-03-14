#include <iostream>
#include <stdexcept>

#include "standard_allocator.hpp"
#include "./logger/standard_logger.hpp"
#include "./logger/standard_logger_builder.hpp"

int main() {
  nstandards::standard_allocator allocator;

  try {
    // ===
    // int* array_of_ints = (int*)allocator.allocate(10 * sizeof(int));
    // int* array_of_ints = (int*)(allocator += 40);
    int* array_of_ints = nullptr;
    allocator += { (void**)&array_of_ints, 10 * sizeof(int) };
    for (int i = 0; i < 10; ++i) {
      array_of_ints[i] = i;
    }
    for (int i = 0; i < 10; ++i) {
      std::cout << array_of_ints[i] << ' ';
    }
    std::cout << std::endl;

    // allocator.deallocate(array_of_ints);
    allocator -= array_of_ints;
    // ===

    // ===

    char* my_str = nullptr;
    allocator += { (void**)&my_str, 13 };
    strcpy(my_str, "Hello World!");
    std::cout << my_str << std::endl;
    allocator -= my_str;

    // ===

    // ===

    int* number = nullptr;
    allocator += { (void**)&number, sizeof(int) };
    *number = 123;
    std::cout << *number << std::endl;
    allocator -= number;

    // ===
  } catch (std::runtime_error error) {
    std::cerr << "An error occured: " << error.what() << std::endl;
  }

  return  0;
}

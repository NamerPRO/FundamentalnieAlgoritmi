#include "standard_allocator.hpp"

nstandards::standard_allocator::standard_allocator() {
  nlogger::logger_builder* builder = nullptr;
  try {
    builder = new standard_logger_builder();
  } catch (std::bad_alloc const &) {
    throw std::runtime_error("An error occured while trying to allocate memory for logger.");
  }
  memory_logger = builder->bind("logs.txt", nlogger::logger::severity::debug)
                      ->build();
  delete builder;
}

nstandards::standard_allocator::~standard_allocator() {
  delete memory_logger;
}

void* nstandards::standard_allocator::allocate(int target_size) const {
  memory_logger->log("Allocating memory for block of size " + std::to_string(target_size) + ".", nlogger::logger::severity::information);
  if (target_size <= 0) {
    memory_logger->log("target_size cannot be negative, but " + std::to_string(target_size) + " found!", nlogger::logger::severity::error);
    throw std::runtime_error("Cannot allocate memory of negative size!");
  }
  void* mem;
  try {
     mem = static_cast<void*>(new char[target_size]);
  } catch (std::bad_alloc error) {
    memory_logger->log("Memory was not allocated due to lack of it.", nlogger::logger::severity::error);
    throw std::runtime_error("Cannot allocate memory of size " + std::to_string(target_size) + "!");
  }
  memory_logger->log("Memory allocated (addr = " + std::to_string(reinterpret_cast<long long>(mem)) + ").", nlogger::logger::severity::information);
  return mem;
}

void nstandards::standard_allocator::deallocate(void const * const target_to_deallocate) {
  memory_logger->log("Deallocating memory (addr = " + std::to_string(reinterpret_cast<long long>(target_to_deallocate)) + ").", nlogger::logger::severity::information);
  // char* cc = (char*)target_to_deallocate;
  int block_bytes_size;
  #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  block_bytes_size = _msize(const_cast<void*>(target_to_deallocate));
  #elif defined(__linux__) || defined(__unix__) || defined(__unix) || defined(unix)
  block_bytes_size = malloc_usable_size(const_cast<void*>(target_to_deallocate));
  #elif defined(__APPLE__)
  block_bytes_size = malloc_size(target_to_deallocate);
  #endif
  std::string bytes_collection = "";
  char* byte_getter = reinterpret_cast<char*>(const_cast<void*>(target_to_deallocate));
  // char* byte_getter = (char*)target_to_deallocate;
  for (int i = 0; i < block_bytes_size; i += 8) {
    // bytes_collection += to_string((*byte_getter));
    bytes_collection += std::to_string(static_cast<int>(*byte_getter)) + ' ';
    ++byte_getter;
  }
  memory_logger->log("Byte representation: " + bytes_collection, nlogger::logger::severity::information);
  delete[] reinterpret_cast<const char*>(target_to_deallocate);
}

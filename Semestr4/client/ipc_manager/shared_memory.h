#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

#include <Windows.h>

#include <stdexcept>
#include <cstring>
#include <utility>

#include "../ipc_manager/semaphores.h"

namespace npipeline_tools {

    class shared_memory_manager {

    private:

        int const _standard_block_size = 1024;

        HANDLE _shared_memory_block;

        char * _shared_memory;

        int const _standard_uuid_size = 36;

    private:

        shared_memory_manager() {
            HANDLE shared_memory_block = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, _standard_block_size, "shared_memory_1");
            if (shared_memory_block == NULL) {
                throw std::runtime_error("An error occured while attempting to create shared memory block.");
            }

            void * shared_memory = MapViewOfFile(shared_memory_block, FILE_MAP_ALL_ACCESS, 0, 0, _standard_block_size);
            if (shared_memory == NULL) {
                CloseHandle(shared_memory_block);
                throw std::runtime_error("An error occured while attempting to create shared memory.");
            }

            _shared_memory_block = shared_memory_block;
            _shared_memory = reinterpret_cast<char *>(shared_memory);
        }

    public:

        shared_memory_manager(
            shared_memory_manager const & other
        ) = delete;

        shared_memory_manager & operator=(
            shared_memory_manager const & other
        ) const = delete;

        static shared_memory_manager & get_instance() {
            static shared_memory_manager signelton_shared_memory_manager;
            return signelton_shared_memory_manager;
        }
        
        char * get_shared_memory() {
            return _shared_memory;
        }

        void free_shared_memory() {
            UnmapViewOfFile(_shared_memory);
            CloseHandle(_shared_memory_block);
        }

        char * write(
            std::string const & serialized_message,
            char * uuid_ptr
        ) {
            char * raw_message = const_cast<char *>(serialized_message.c_str());
            int message_size = serialized_message.size();

            // Sending message to server
            while (message_size > 0) {
                semaphore_manager & semaphores = semaphore_manager::get_instance();

                // Stay blocked until we can write something
                if (WaitForSingleObject(semaphores.get(semaphore_manager::type::client), INFINITE) != 0) {
                    throw std::runtime_error("An error occured while attemption to decrement semaphore!");
                }

                // Set mark whether we have uuid
                _shared_memory[1] = (uuid_ptr == nullptr) ? 'g' : 'n';

                // Amount of character we can write to shared memory
                int characters_to_copy = _standard_block_size - 1;
                
                // If message cannot be sent fully, send it by parts
                if (message_size > characters_to_copy - _standard_uuid_size) {
                    _shared_memory[0] = 'a';
                    --characters_to_copy;

                    // Write data to shared memory
                    if (uuid_ptr == nullptr) {
                        std::memcpy(_shared_memory + 2, raw_message, sizeof(char) * characters_to_copy);
                    } else {
                        std::memcpy(_shared_memory + 2, uuid_ptr, sizeof(char) * _standard_uuid_size);
                        characters_to_copy -= _standard_uuid_size;
                        std::memcpy(_shared_memory + 2 + _standard_uuid_size, raw_message, sizeof(char) * characters_to_copy);
                    }
                } else {
                    _shared_memory[0] = 'f';
                    if (uuid_ptr == nullptr) {
                        std::memcpy(_shared_memory + 2, raw_message, sizeof(char) * message_size);
                    } else {
                        std::memcpy(_shared_memory + 2, uuid_ptr, sizeof(char) * _standard_uuid_size);
                        std::memcpy(_shared_memory + 2 + _standard_uuid_size, raw_message, sizeof(char) * message_size);
                    }
                    characters_to_copy = message_size;
                }
                raw_message += characters_to_copy;
                message_size -= characters_to_copy;
                
                // Unlock server as shared memory now contains our data
                if (ReleaseSemaphore(semaphores.get(semaphore_manager::type::server), 1, NULL) == 0) {
                    throw std::runtime_error("An error occured while attemption to release semaphore!");
                }
                
                // Block client so it does not destroy data in shared memory while server is working with it
                if (WaitForSingleObject(semaphores.get(semaphore_manager::type::certain_client), INFINITE) != 0) {
                    throw std::runtime_error("An error occured while attemption to decrement semaphore!");
                }

                if (uuid_ptr == nullptr) {
                    uuid_ptr = new char[_standard_block_size + 1];
                    std::memcpy(uuid_ptr, _shared_memory, sizeof(char) * _standard_uuid_size);
                    uuid_ptr[_standard_uuid_size] = '\0';
                }

                // Certain client got uuid if needed and now clients can be unlocked
                if (ReleaseSemaphore(semaphores.get(semaphore_manager::type::client), 1, NULL) == 0) {
                    throw std::runtime_error("An error occured while attemption to release semaphore!");
                }
            }

            return uuid_ptr;
        }

        std::pair<bool, std::string> ping(
            char * uuid_ptr
        ) {
            semaphore_manager & semaphores = semaphore_manager::get_instance();
            
            // Stay blocked until we can write something
            if (WaitForSingleObject(semaphores.get(semaphore_manager::type::client), INFINITE) != 0) {
                throw std::runtime_error("An error occured while attemption to decrement semaphore!");
            }

            _shared_memory[0] = 'r';
            _shared_memory[1] = 'n';
            std::memcpy(_shared_memory + 2, uuid_ptr, sizeof(char) * _standard_uuid_size);

            // Unlock server
            if (ReleaseSemaphore(semaphores.get(semaphore_manager::type::server), 1, NULL) == 0) {
                throw std::runtime_error("An error occured while attemption to release semaphore!");
            }

            // Stay blocked until server sends a response
            if (WaitForSingleObject(semaphores.get(semaphore_manager::type::certain_client), INFINITE) != 0) {
                throw std::runtime_error("An error occured while attemption to decrement semaphore!");
            }

            bool is_response_positive = _shared_memory[0] == 'y';

            std::string response_string;

            // If server answer it has a result, then it will return it instantly
            if (is_response_positive) {
                do {
                    if (WaitForSingleObject(semaphores.get(semaphore_manager::type::certain_client), INFINITE) != 0) {
                        throw std::runtime_error("An error occured while attemption to decrement semaphore!");
                    }
                    for (int i = 1; i < _standard_block_size; ++i) {
                        response_string += _shared_memory[i];
                    }
                    if (ReleaseSemaphore(semaphores.get(semaphore_manager::type::server), 1, NULL) == 0) {
                        throw std::runtime_error("An error occured while attemption to release semaphore!");
                    }
                } while (_shared_memory[0] == 'a');
            }

            if (ReleaseSemaphore(semaphores.get(semaphore_manager::type::client), 1, NULL) == 0) {
                throw std::runtime_error("An error occured while attemption to release semaphore!");
            }

            return { is_response_positive, response_string };
        }

    };

}

#endif // SHARED_MEMORY_H_
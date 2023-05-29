#include "./shared_memory.h"

namespace npipeline_tools {

    int shared_memory_manager::get_standard_uuid_size() {
        return _standard_uuid_size;
    }

    int shared_memory_manager::get_standard_block_size() {
        return _standard_block_size;
    }

    shared_memory_manager::shared_memory_manager() {
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

    shared_memory_manager & shared_memory_manager::get_instance() {
        static shared_memory_manager signelton_shared_memory_manager;
        return signelton_shared_memory_manager;
    }
        
    char * shared_memory_manager::get_shared_memory() {
        return _shared_memory;
    }

    void shared_memory_manager::free_shared_memory() {
        UnmapViewOfFile(_shared_memory);
        CloseHandle(_shared_memory_block);
    }

    void shared_memory_manager::write(
        std::string const & serialized_message
    ) {
        char * raw_message = const_cast<char *>(serialized_message.c_str());
        int message_size = serialized_message.size();

        semaphore_manager & semaphores = semaphore_manager::get_instance();

        // Sending message to server
        while (message_size > 0) {

            // Set flag whether we finished sending message by parts 
            _shared_memory[0] = (message_size > _standard_block_size - 1) ? 'a' : 'f';

            // Send message
            std::memcpy(_shared_memory, raw_message, sizeof(char) * (_standard_block_size - 1));
            
            raw_message += _standard_block_size - 1;
            message_size -= _standard_block_size - 1;
                
            // Unlock client so it can read data in shared memory
            if (ReleaseSemaphore(semaphores.get(semaphore_manager::type::certain_client), 1, NULL) == 0) {
                throw std::runtime_error("An error occured while attemption to release semaphore!");
            }
                
            // Wait till client finishes working with sent data
            if (WaitForSingleObject(semaphores.get(semaphore_manager::type::server), INFINITE) != 0) {
                throw std::runtime_error("An error occured while attemption to decrement semaphore!");
            }

        }
    }

}
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

        static int const _standard_block_size = 1024;

        HANDLE _shared_memory_block;

        char * _shared_memory;

        static int const _standard_uuid_size = 36;
    
    public:

        static int get_standard_uuid_size();

        static int get_standard_block_size();

    private:

        shared_memory_manager();

    public:

        shared_memory_manager(
            shared_memory_manager const & other
        ) = delete;

        shared_memory_manager & operator=(
            shared_memory_manager const & other
        ) const = delete;

        static shared_memory_manager & get_instance();
        
        char * get_shared_memory();

        void free_shared_memory();

        void write(
            std::string const & serialized_message
        );

    };

}

#endif // SHARED_MEMORY_H_
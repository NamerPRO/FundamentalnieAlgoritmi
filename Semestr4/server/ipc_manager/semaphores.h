#ifndef SEMAPHORES_H_
#define SEMAPHORES_H_

#include <Windows.h>

#include <vector>
#include <string>
#include <stdexcept>

namespace npipeline_tools {

    class semaphore_manager {

    private:

        std::vector<HANDLE> _vector_with_semaphores;

        semaphore_manager();

    public:

        static semaphore_manager & get_instance();

        enum class type {
            client = 0,
            server = 1,
            certain_client = 2
        };

        HANDLE & get(
            type semaphore_type
        );

        ~semaphore_manager();

    };

}

#endif // SEMAPHORES_H_
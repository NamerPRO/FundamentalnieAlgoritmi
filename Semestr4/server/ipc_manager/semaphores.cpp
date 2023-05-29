#include "./semaphores.h"

namespace npipeline_tools {

    semaphore_manager::semaphore_manager() {
        std::vector<int> initial_semaphores_count = { 1, 0, 0 };
        for (int i = 0; i < 3; ++i) {
            HANDLE semaphore = CreateSemaphore(NULL, initial_semaphores_count[i], 1, ("semaphore" + std::to_string(i)).data());
            if (semaphore == 0) {
                throw std::runtime_error("Failed to access semaphore!");
            }
            _vector_with_semaphores[i] = semaphore;
        }
    }

    HANDLE & semaphore_manager::get(
        type semaphore_type
    ) {
        return _vector_with_semaphores[static_cast<int>(semaphore_type)];
    }

    semaphore_manager::~semaphore_manager() {
        for (int i = 0; i < 3; ++i) {
            CloseHandle(_vector_with_semaphores[i]);
        }
    }

    semaphore_manager & semaphore_manager::get_instance() {
        static semaphore_manager sigelton_semaphore_manager;
        return sigelton_semaphore_manager;
    }

}
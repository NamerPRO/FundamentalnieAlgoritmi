#include <iostream>
#include <stdexcept>

#include "./navl/avl_tree.h"
#include "./nbst/binary_search_tree.h"
#include "./nmemory/allocate_type_helper.h"
#include "./nmemory/memory.h"
#include "./nsplay/splay_tree.h"
#include "./pipeline_manager/pipeline.h"
#include "./nmemory/memory_boundary_descriptors.h"
#include "./nmemory/buddy_memory_allocator.h"
#include "./nmemory/sorted_list_memory.h"

// CHANGE INSERT FUNCTION IN BTREE!!! (>>>not needed<<<)
// PASS ALLOCATOR IN INSERT COMMANDS, SO INNER TREES CAN USE IT!!! (>>>done<<<)
// FINISH NUMBER 3!!! (>>>done<<<)
// implement flyweight pattern for every string in interpriter commands (standard_pipeline_inter...)

#include "./flyweight_string/string_manager.h"

int main(
    int argc,
    char * argv[]
) { 
    nmemory::memory * my_allocator;

    try {
        my_allocator = new nmemory_standards::boundary_descriptors_memory(
            nullptr,
            9999999,
            nmemory_standards::allocate_type_helper::allocate_type::first_fit,
            nullptr
        );
    } catch (std::bad_alloc &) {
        std::cout << "Memory allocate exception! Failed to allocate memory for allocator.";
        return 0;
    }
    
    npipeline::pipeline * my_pipe;

    try {
        switch (argc) {

        case 1: {
            my_pipe = new npipeline::pipeline(
                npipeline::pipeline_base::tree_type::avl,
                npipeline::pipeline_base::interpriter_type::user_input_interpriter
            );
            break;
        }

        case 2: {
            my_pipe = new npipeline::pipeline(
                npipeline::pipeline_base::tree_type::avl,
                npipeline::pipeline_base::interpriter_type::file_input_interpriter,
                argv[1],
                my_allocator
            );
            break;
        }

        default: {
            throw std::runtime_error("Wrong arguments exception! Program requires ONE file with commands to run.");
        }

        }
    } catch (std::bad_alloc &) {
        std::cout << "Memory allocate exception! Failed to allocate memory for pipeline." << std::endl;
        delete my_allocator;
        return 0;
    } catch (std::runtime_error & error) {
        std::cout << error.what() << std::endl;
        delete my_allocator;
        return 0;
    }

    try {
        my_pipe->run();
    } catch (std::runtime_error & error) {
        std::cout << error.what() << std::endl;
    }

    delete my_pipe;
    delete my_allocator;

    return 0;
}
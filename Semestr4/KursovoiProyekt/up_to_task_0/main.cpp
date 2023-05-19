#include <iostream>
#include <stdexcept>

#include "navl/avl_tree.h"
#include "nbst/binary_search_tree.h"
#include "nmemory/allocate_type_helper.h"
#include "nmemory/memory.h"
#include "pipeline_manager/pipeline.h"
#include "nmemory/memory_boundary_descriptors.h"

int main(int argc, char * argv[]) {

    /*nmemory::memory * my_allocator = new nmemory_standards::boundary_descriptors_memory(
        nullptr,
        9999999,
        nmemory_standards::allocate_type_helper::allocate_type::first_fit,
        nullptr
    );

    npipeline::pipeline * my_pipe;

    switch (argc) {

    case 1: {
        my_pipe = new npipeline::pipeline(
            npipeline::pipeline::tree_type::avl,
            npipeline::pipeline::interpriter_type::user_input_interpriter
        );
        break;
    }

    case 2: {
        my_pipe = new npipeline::pipeline(
            npipeline::pipeline::tree_type::avl,
            npipeline::pipeline::interpriter_type::file_input_interpriter,
            argv[1],
            my_allocator
        );
        break;
    }

    default: {
        throw std::runtime_error("Wrong arguments exception! Program requires ONE file with commands to run.");
    }

    }

    my_pipe->run();

     // "/home/petera/Рабочий стол/FundamentalnieAlgoritmi/Semestr4/KursovoiProyekt/up_to_task_0/build/insts"

     */

    navl::avl_tree<int, std::string> tree;

    return 0;
}
#include <iostream>
#include <stdexcept>

#include "navl/avl_tree.h"
#include "pipeline_manager/pipeline.h"

int main(int argc, char * argv[]) {

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
            argv[1]
        );
        break;
    }

    default: {
        throw std::runtime_error("Wrong arguments exception! Program requires ONE file with commands to run.");
    }

    }

    my_pipe->run();

     // "/home/petera/Рабочий стол/FundamentalnieAlgoritmi/Semestr4/KursovoiProyekt/up_to_task_0/build/insts"



    return 0;
}
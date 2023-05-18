#include <iostream>
#include <stdexcept>

#include "navl/avl_tree.h"
#include "pipeline_manager/pipeline.h"

int main(int argc, char * argv[]) {

    if (argc != 2) {
        throw std::runtime_error("Wrong arguments exception! Program requires ONE file with commands to run.");
    }

    npipeline::pipeline my_pipe(npipeline::pipeline::tree_type::avl);
    my_pipe.run(argv[1]); // "/home/petera/Рабочий стол/FundamentalnieAlgoritmi/Semestr4/KursovoiProyekt/up_to_task_0/build/insts"



    return 0;
}
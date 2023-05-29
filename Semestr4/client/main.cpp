#include <iostream>
#include <stdexcept>

#include "./interpriter_manager/interpriter.h"
#include "./interpriter_manager/pipeline_interpriters.h"

int main(
    int argc,
    char * argv[]
) {
    ninterpriter::interpriter * interpriter;

    switch (argc) {

    case 1: {
        interpriter = new npipeline_tools::pipeline_interpriter(argv[1]);
    }

    case 2: {
        interpriter = new npipeline_tools::user_interpriter();
    }

    default: {
        throw std::runtime_error("Failed to start due to wrong arguments passed!");
    }

    }

    interpriter->interpritate();

    delete interpriter;

    return 0;
}
#include "pipeline.h"
#include <ios>
#include <vector>

namespace npipeline {

    void pipeline::user_interpriter::interpritate() {
        std::vector<std::string> tokens_list(17); // Vector cannot contain more than 17 elements
        std::cout << "Enter an action: ";
        std::noskipws(std::cin);
        while (std::getline(std::cin, tokens_list[0])) {
            
            if (tokens_list[0].starts_with("add")) {
                std::cout << "Enter pool: ";
                std::getline(std::cin, tokens_list[1]);
                if (!tokens_list[0].ends_with("pool")) {
                    std::cout << "Enter scheme: ";
                    std::getline(std::cin, tokens_list[2]);
                    if (!tokens_list[0].ends_with("scheme")) {
                        std::cout << "Enter collection: ";
                        std::getline(std::cin, tokens_list[3]);
                        if (tokens_list[0].ends_with("note")) {
                            std::cout << "Enter build id: ";
                            std::getline(std::cin, tokens_list[4]);
                            std::cout << "Enter build version: ";
                            std::getline(std::cin, tokens_list[5]);
                            tokens_list[6] = tokens_list[4];
                            tokens_list[7] = tokens_list[5];
                            std::cout << "Enter commit hash: ";
                            std::getline(std::cin, tokens_list[8]);
                            std::cout << "Enter developer login: ";
                            std::getline(std::cin, tokens_list[9]);
                            std::cout << "Enter developer email: ";
                            std::getline(std::cin, tokens_list[10]);
                            std::cout << "Enter build script link: ";
                            std::getline(std::cin, tokens_list[11]);
                            std::cout << "Enter build name: ";
                            std::getline(std::cin, tokens_list[12]);
                            std::cout << "Enter build error information: ";
                            std::getline(std::cin, tokens_list[13]);
                            std::cout << "Enter code analysis information: ";
                            std::getline(std::cin, tokens_list[14]);
                            std::cout << "Enter test error information: ";
                            std::getline(std::cin, tokens_list[15]);
                            std::cout << "Enter link to artifacts: ";
                            std::getline(std::cin, tokens_list[16]);
                        }
                    }
                }
            } else if (tokens_list[0].starts_with("remove")) {
                std::cout << "Enter pool: ";
                std::getline(std::cin, tokens_list[1]);
                if (!tokens_list[0].ends_with("pool")) {
                    std::cout << "Enter scheme: ";
                    std::getline(std::cin, tokens_list[2]);
                    if (!tokens_list[0].ends_with("scheme")) {
                        std::cout << "Enter collection: ";
                        std::getline(std::cin, tokens_list[3]);
                        if (tokens_list[0].ends_with("note")) {
                            std::cout << "Enter build id: ";
                            std::getline(std::cin, tokens_list[4]);
                            std::cout << "Enter build version: ";
                            std::getline(std::cin, tokens_list[5]);
                        }
                    }
                }
            } else if (tokens_list[0].starts_with("read")) {
                std::cout << "Enter pool: ";
                std::getline(std::cin, tokens_list[1]);
                std::cout << "Enter scheme: ";
                std::getline(std::cin, tokens_list[2]);
                std::cout << "Enter collection: ";
                std::getline(std::cin, tokens_list[3]);
                std::cout << "Enter start build id: ";
                std::getline(std::cin, tokens_list[4]);
                std::cout << "Enter start build version: ";
                std::getline(std::cin, tokens_list[5]);
                if (tokens_list[0].ends_with("range")) {
                    std::cout << "Enter end build id: ";
                    std::getline(std::cin, tokens_list[6]);
                    std::cout << "Enter end build version: ";
                    std::getline(std::cin, tokens_list[7]);
                }
            } else if (tokens_list[0].starts_with("run")) {
                std::cout << "Enter path to file: ";
                std::getline(std::cin, tokens_list[1]);
                run_file(_dbase, tokens_list);
                tokens_list.clear();
                std::cout << "Enter an action: ";
                continue;
            }
            for (int i = 0; i < _actions.size(); ++i) {
                if (_commands[i] == tokens_list[0]) {
                    _actions[i](_dbase, tokens_list);
                }
            }
            tokens_list.clear();
            std::cout << "Enter an action: ";
        }
        std::cout << std::endl;
    }

    void pipeline::user_interpriter::run_file(
        data_base * dbase,
        std::vector<std::string> const & data
    ) {
        ninterpritator::interpritator * temp_interpriter = new pipeline_interpriter(dbase, data[1]);
        temp_interpriter->interpritate();
        delete temp_interpriter;
    }

}
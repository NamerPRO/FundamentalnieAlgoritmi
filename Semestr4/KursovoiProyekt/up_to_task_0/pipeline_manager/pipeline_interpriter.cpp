#include "./pipeline.h"

namespace npipeline {

    // Assuming that file has valid syntax
    void pipeline_interpriter::interpritate() {
        std::ifstream file(_file_to_interpritate_path);
        if (!file.is_open()) {
            throw std::runtime_error("File to interpritate cannot be opened! Does it exist at given path?");
        }
        std::string token;
        std::vector<std::string> tokens_list;
        char symbol;
        bool allow_spaces = false;
        while (file >> std::noskipws >> symbol) {
            if (symbol == ' ' && !allow_spaces) {
                continue;
            }

            switch (symbol) {
            
            case '&':
                allow_spaces = true;
                tokens_list.push_back(std::move(token));
                break;

            case '-':
            case '.':
            case ':':
            case '/':
            case ',':
            case '\n':
            case EOF:
                allow_spaces = false;
                tokens_list.push_back(std::move(token));
                break;

            default:
                token.push_back(symbol);
                break;

            }

            if (symbol == '\n' || symbol == EOF) {

                if (!tokens_list[0].empty()) {
                    ninterpritator::interpritator::command * cmd = _commands[tokens_list[0]]->create_command(_dbase, _dbase_with_developer_login_key, tokens_list);

                    _invoker->add(cmd);
                    _invoker->invoke();
                }

                tokens_list.clear();
            }

        }
        file.close();
    }

}
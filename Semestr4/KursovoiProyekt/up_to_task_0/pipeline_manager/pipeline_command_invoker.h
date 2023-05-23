#ifndef PIPELINE_COMMAND_INVOKER
#define PIPELINE_COMMAND_INVOKER

#include <list>

#include "../interpritator_manager/interpritator.h"

namespace npipeline {

    class invoker {

    private:

        std::list<ninterpritator::interpritator::command *> _commands_list;

    public:

        void add(
            ninterpritator::interpritator::command * cmd
        ) {
            _commands_list.push_front(cmd);
        }

        void invoke() {
            while (!_commands_list.empty()) {
                _commands_list.back()->execute();
                delete _commands_list.back();
                _commands_list.pop_back();
            }
        }

        ~invoker() {
            while (!_commands_list.empty()) {
                delete _commands_list.back();
                _commands_list.pop_back();
            }
        }
    
    };

}

#endif /* PIPELINE_COMMAND_INVOKER */

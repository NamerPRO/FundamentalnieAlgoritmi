#ifndef PIPELINE_COMMAND_INVOKER
#define PIPELINE_COMMAND_INVOKER

#include <exception>
#include <list>
#include <thread>
#include <utility>

#include "./base_command.h"

namespace npipeline_tools {

    class invoker {

    private:

        std::list<command *> _commands_queue;
        std::list<command *> _commands_processed;

    public:

        invoker & add(
            command * cmd
        ) {
            _commands_queue.push_front(cmd);
            return *this;
        }

        void invoke() {
            while (!_commands_queue.empty()) {
                // Start a thread with commands
                command * command_instanse = _commands_queue.back();
                std::thread command_thread(&command::execute, command_instanse);

                _commands_processed.push_back(_commands_queue.back());
                _commands_queue.pop_back();
            }
        }

        ~invoker() {
            while (!_commands_processed.empty()) {
                delete _commands_processed.back();
                _commands_processed.pop_back();
            }
            while (!_commands_queue.empty()) {
                delete _commands_queue.back();
                _commands_queue.pop_back();
            }
        }
    
    };

}

#endif /* PIPELINE_COMMAND_INVOKER */

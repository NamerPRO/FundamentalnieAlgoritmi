#ifndef COMMANDS_CREATOR
#define COMMANDS_CREATOR

#include <string>
#include <vector>

#include "../interpritator_manager/interpritator.h"
#include "./pipeline_base.h"

namespace npipeline {

    class command_creator {

    public:

        virtual ninterpritator::interpritator::command * create_command(
            pipeline_base::data_base * dbase,
            std::vector<std::string> & args,
            unsigned long long date
        ) = 0;

        virtual ~command_creator() = default;

    };

}

#endif /* COMMANDS_CREATOR */

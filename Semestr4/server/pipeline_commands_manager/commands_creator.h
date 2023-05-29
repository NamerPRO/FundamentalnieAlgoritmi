#ifndef COMMANDS_CREATOR
#define COMMANDS_CREATOR

#include <string>
#include <vector>

#include "./base_command.h"

#include "../nmemory/memory.h"

#include "../pipeline_manager/pipeline_base.h"

namespace npipeline_tools {

    class command_creator {

    public:

        virtual command * create_command(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::vector<std::string> & args,
            nmemory::memory * allocator,
            std::string & uuid_as_string
        ) = 0;

        virtual ~command_creator() = default;

    };

}

#endif /* COMMANDS_CREATOR */

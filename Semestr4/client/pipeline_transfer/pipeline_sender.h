#ifndef PIPELINE_COMMANDS_SENDER
#define PIPELINE_COMMANDS_SENDER

#include <exception>
#include <list>
#include <vector>
#include <string>

#include "../interpriter_manager/interpriter.h"

#include "./pipeline_serializer.h"

#include "../ipc_manager/shared_memory.h"

#include "../transfer_manager/standard_sender.h"

namespace npipeline_tools {

    class sender final : public ntransfer::standard_sender<sender> {

    private:

        std::list<std::string> _requests_list;

    public:

        sender & add(
            std::vector<std::string> const & parsed_cmd
        ) override;

        char * send(
            char * uuid
        ) override;

        ~sender() = default;
    
    };

}

#endif /* PIPELINE_COMMANDS_SENDER */

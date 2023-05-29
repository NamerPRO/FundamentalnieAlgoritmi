#ifndef DATA_STORAGE_H_
#define DATA_STORAGE_H_

#include <string>
#include <unordered_map>
#include <memory>

#include "../pipeline_manager/pipeline.h"

#include "../pipeline_commands_manager/commands_creator.h"
#include "../pipeline_commands_manager/pipeline_commands.h"

namespace npipeline_tools {

    class storage {
    
    private:

        // Hash table with requests and responses to them
        std::unordered_map<std::string, std::string> _requests;

        // Hash table with responses
        std::unordered_map<std::string, std::string> _responses;

        // Data bases storage
        std::unordered_map<std::string, npipeline::pipeline> _pipelines;

        // Commands
        std::unordered_map<std::string, command_creator *>  _actions = {
            { "ADD_POOL", new add_pool::creator },
            { "ADD_SCHEME", new add_scheme::creator },
            { "ADD_COLLECTION", new add_collection::creator },
            { "ADD_OR_UPDATE_NOTE", new add_or_update_note::creator },
            { "READ_NOTE", new read_note::creator },
            { "READ_NOTE_TIMED", new read_note_timed::creator },
            { "REMOVE_NOTE", new remove_note::creator },
            { "REMOVE_COLLECTION", new remove_collection::creator },
            { "REMOVE_SCHEME", new remove_scheme::creator },
            { "REMOVE_POOL", new remove_pool::creator },
            { "READ_IN_RANGE", new read_in_range::creator }
            // { "RUN_FILE", new run_file::creator }
        };

    private:

        storage() {}

    public:

        static storage & get_instanse();

        storage(
            storage const & other
        ) = delete;

        storage & operator=(
            storage const & other
        ) const = delete;

    public:

        std::unordered_map<std::string, std::string> & get_requests();

        std::unordered_map<std::string, std::string> & get_responses();

        std::unordered_map<std::string, npipeline::pipeline> & get_pipelines();

        std::unordered_map<std::string, command_creator *> & get_actions();

        ~storage();

    };

}

#endif // DATA_STORAGE_H_
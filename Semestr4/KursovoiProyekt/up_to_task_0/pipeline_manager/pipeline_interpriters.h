#ifndef PIPELINE_INTERPRITERS
#define PIPELINE_INTERPRITERS

#include <map>
#include <vector>

#include "./pipeline_base.h"
#include "./pipeline_command_invoker.h"
#include "./commands_creator.h"
#include "./standard_pipeline_interpriter_commands.h"

namespace npipeline {

    class pipeline_interpriter final : public ninterpritator::interpritator {

    private:

        pipeline_base::data_base * _dbase;

        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _file_to_interpritate_path;

        invoker * _invoker;

    public:

        explicit pipeline_interpriter(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & file_to_interpritate_path,
            invoker * invoker
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _file_to_interpritate_path(std::move(file_to_interpritate_path)), _invoker(invoker) {}

        void interpritate() override;

        std::map<std::string, command_creator *>  _commands = {
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
            { "READ_IN_RANGE", new read_in_range::creator },
            { "RUN_FILE", new run_file::creator }
        };

        ~pipeline_interpriter() {
            auto it_end = _commands.end();
            for (auto it_begin = _commands.begin(); it_begin != it_end; ++it_begin) {
                delete it_begin->second;
            }
        }

    };

    class user_interpriter final : public ninterpritator::interpritator {
            
    private:

        pipeline_base::data_base * _dbase;

        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        invoker * _invoker;

    public:

        explicit user_interpriter(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            invoker * invoker
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _invoker(invoker) {}

        void interpritate() override;

        std::map<std::string, command_creator *>  _commands = {
            { "add pool", new add_pool::creator },
            { "add scheme", new add_scheme::creator },
            { "add collection", new add_collection::creator },
            { "add or update note", new add_or_update_note::creator },
            { "read note", new read_note::creator },
            { "read note timed", new read_note_timed::creator },
            { "remove note", new remove_note::creator },
            { "remove collection", new remove_collection::creator },
            { "remove scheme", new remove_scheme::creator },
            { "remove pool", new remove_pool::creator },
            { "read in range", new read_in_range::creator },
            { "run file", new run_file::creator }
        };

        ~user_interpriter() {
            auto it_end = _commands.end();
            for (auto it_begin = _commands.begin(); it_begin != it_end; ++it_begin) {
                delete it_begin->second;
            }
        }

    };

}

#endif /* PIPELINE_INTERPRITERS */

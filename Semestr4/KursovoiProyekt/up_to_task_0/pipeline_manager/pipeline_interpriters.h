#ifndef PIPELINE_INTERPRITERS
#define PIPELINE_INTERPRITERS

#include <vector>

#include "./pipeline_base.h"
#include "./pipeline_command_invoker.h"

namespace npipeline {

    class pipeline_interpriter final : public ninterpritator::interpritator {

    private:

        pipeline_base::data_base * _dbase;

        std::string _file_to_interpritate_path;

        invoker * _invoker;

    public:

        explicit pipeline_interpriter(
            pipeline_base::data_base * dbase,
            std::string & file_to_interpritate_path,
            invoker * invoker
        ) : _dbase(dbase), _file_to_interpritate_path(std::move(file_to_interpritate_path)), _invoker(invoker) {}

        void interpritate() override;

        ~pipeline_interpriter() = default;

        std::vector<std::string> _commands = { "ADD_POOL", "ADD_SCHEME", "ADD_COLLECTION", "ADD_OR_UPDATE_NOTE", "READ_NOTE", "REMOVE_NOTE", "REMOVE_COLLECTION", "REMOVE_SCHEME", "REMOVE_POOL", "READ_IN_RANGE", "RUN_FILE" };

    };

    class user_interpriter final : public ninterpritator::interpritator {
            
    private:

        pipeline_base::data_base * _dbase;

        invoker * _invoker;

    public:

        explicit user_interpriter(
            pipeline_base::data_base * dbase,
            invoker * invoker
        ) : _dbase(dbase), _invoker(invoker) {}

        void interpritate() override;

        ~user_interpriter() = default;

        std::vector<std::string> _commands = { "add pool", "add scheme", "add collection", "add or update note", "read note", "remove note", "remove collection", "remove scheme", "remove pool", "read in range", "run file" };

    };
    
    // ===

}

#endif /* PIPELINE_INTERPRITERS */

#ifndef STANDARD_PIPELINE_INTERPRITER_COMMANDS
#define STANDARD_PIPELINE_INTERPRITER_COMMANDS

// #include "./pipeline.h"
#include <stdexcept>
#include <string>
#include <vector>

#include "../interpritator_manager/interpritator.h"
#include "./pipeline_interpriters.h"
#include "./pipeline_command_invoker.h"
#include "./pipeline_base.h"

namespace npipeline {

    class add_pool final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;

    public:

        explicit add_pool(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)) {}

        void execute() override;

        ~add_pool() = default;

    };

    class remove_pool final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;

    public:

        explicit remove_pool(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)) {}

        void execute() override;

        ~remove_pool() = default;

    };

    class add_scheme final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;

    public:

        explicit add_scheme(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)) {}

        void execute() override;

        ~add_scheme() = default;

    };

    class remove_scheme final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;

    public:

        explicit remove_scheme(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)) {}

        void execute() override;

        ~remove_scheme() = default;

    };

    class add_collection final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;

    public:

        explicit add_collection(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)) {}

        void execute() override;

        ~add_collection() = default;

    };

    class remove_collection final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;

    public:

        explicit remove_collection(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)) {}

        void execute() override;

        ~remove_collection() = default;

    };

    class add_or_update_note final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        unsigned long _build_id;
        unsigned long _build_version;
        unsigned long _commit_hash;
        std::string _developer_login;
        std::string _developer_email;
        std::string _build_script_link;
        std::string _build_name;
        std::string _build_error_information;
        std::string _code_analysis_information;
        std::string _test_error_information;
        std::string _link_to_artifacts;

    public:

        explicit add_or_update_note(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long commit_hash,
            std::string & developer_login,
            std::string & developer_email,
            std::string & build_script_link,
            std::string & build_name,
            std::string & build_error_information,
            std::string & code_analysis_information,
            std::string & test_error_information,
            std::string & link_to_artifacts,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version), _commit_hash(commit_hash),
            _developer_login(std::move(developer_login)), _developer_email(std::move(developer_email)),
            _build_script_link(std::move(build_script_link)), _build_name(std::move(build_name)),
            _build_error_information(std::move(build_error_information)), _code_analysis_information(std::move(code_analysis_information)),
            _test_error_information(std::move(test_error_information)), _link_to_artifacts(std::move(link_to_artifacts)) {}

        void execute() override;

        ~add_or_update_note() = default;

    };

    class read_note final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        unsigned long _build_id;
        unsigned long _build_version;

    public:

        explicit read_note(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version) {}

        void execute() override;

        ~read_note() = default;

    };

    class remove_note final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        unsigned long _build_id;
        unsigned long _build_version;

    public:

        explicit remove_note(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version) {}

        void execute() override;

        ~remove_note() = default;

    };

    class read_in_range final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        unsigned long _build_id_start;
        unsigned long _build_version_start;
        unsigned long _build_id_end;
        unsigned long _build_version_end;

    public:

        explicit read_in_range(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id_start,
            unsigned long build_version_start,
            unsigned long build_id_end,
            unsigned long build_version_end,
            unsigned long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id_start(build_id_start), _build_version_start(build_version_start),
            _build_id_end(build_id_end), _build_version_end(build_version_end) {}

        void execute() override;

        ~read_in_range() = default;

    };

    class run_file final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _path_to_file;
        invoker * _cmd_invoker;

    public:

        explicit run_file(
            npipeline::pipeline_base::data_base * dbase,
            std::string & path_to_file,
            invoker * cmd_invoker,
            unsigned long date
        ) : _date(date), _dbase(dbase), _path_to_file(std::move(path_to_file)),
            _cmd_invoker(new invoker) {}

        void execute() override;

        ~run_file() {
            delete _cmd_invoker;
        }

    };

    class command_helper {

    public:

        static ninterpritator::interpritator::command * get_cmd_ptr_by_name(
            pipeline_base::data_base * dbase,
            std::vector<std::string> & args,
            unsigned long long date,
            std::vector<std::string> & commands,
            invoker * cmd_invoker
        );

    };
    
}

#endif /* STANDARD_PIPELINE_INTERPRITER_COMMANDS */

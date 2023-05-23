#ifndef STANDARD_PIPELINE_INTERPRITER_COMMANDS
#define STANDARD_PIPELINE_INTERPRITER_COMMANDS

#include <ctime>
#include <stdexcept>
#include <string>
#include <vector>

#include "../interpritator_manager/interpritator.h"
#include "./pipeline_command_invoker.h"
#include "./pipeline_base.h"
#include "./commands_creator.h"

namespace npipeline {

    class add_pool final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;

    public:

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new add_pool(dbase, args[1], date);
            }

        };

        explicit add_pool(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new remove_pool(dbase, args[1], date);
            }

        };

        explicit remove_pool(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new add_scheme(dbase, args[1], args[2], date);
            }

        };

        explicit add_scheme(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new remove_scheme(dbase, args[1], args[2], date);
            }

        };

        explicit remove_scheme(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new add_collection(dbase, args[1], args[2], args[3], date);
            }

        };

        explicit add_collection(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new remove_collection(dbase, args[1], args[2], args[3], date);
            }

        };

        explicit remove_collection(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new add_or_update_note(dbase, args[1], args[2], args[3], std::stoul(args[6]), std::stoul(args[7]), std::stoul(args[8]), args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16], date);
            }

        };

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
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new read_note(dbase, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), date);
            }

        };

        explicit read_note(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version) {}

        void execute() override;

        ~read_note() = default;

    };

    class read_note_timed final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date;

        npipeline::pipeline_base::data_base * _dbase;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        unsigned long _build_id;
        unsigned long _build_version;
        unsigned long long _time;

    public:

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new read_note_timed(dbase, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), to_milliseconds(std::stoi(args[6]), std::stoi(args[7]), std::stoi(args[8]), std::stoi(args[9]), std::stoi(args[10]), std::stoi(args[11])), date);
            }

        };

        static unsigned long long to_milliseconds(
            int day,
            int month,
            int year,
            int hours,
            int minutes,
            int seconds
        ) {
            std::tm time = {
                .tm_sec = seconds,
                .tm_min = minutes,
                .tm_hour = hours,
                .tm_mday = day,
                .tm_mon = month - 1,
                .tm_year = year - 1900
            };
            return std::mktime(&time) * 1000;
        }

        explicit read_note_timed(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long long time,
            unsigned long long date
        ) : _date(date), _dbase(dbase), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version), _time(time) {}

        void execute() override;

        ~read_note_timed() = default;

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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new remove_note(dbase, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), date);
            }

        };

        explicit remove_note(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new read_in_range(dbase, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), std::stoul(args[6]), std::stoul(args[7]), date);
            }

        };

        explicit read_in_range(
            npipeline::pipeline_base::data_base * dbase,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id_start,
            unsigned long build_version_start,
            unsigned long build_id_end,
            unsigned long build_version_end,
            unsigned long long date
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

        class creator final : public npipeline::command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                std::vector<std::string> & args,
                unsigned long long date
            ) {
                return new run_file(dbase, args[1], date);
            }

        };

        explicit run_file(
            npipeline::pipeline_base::data_base * dbase,
            std::string & path_to_file,
            unsigned long long date
        ) : _date(date), _dbase(dbase), _path_to_file(std::move(path_to_file)),
            _cmd_invoker(new invoker) {}

        void execute() override;

        ~run_file() {
            delete _cmd_invoker;
        }

    };
    
}

#endif /* STANDARD_PIPELINE_INTERPRITER_COMMANDS */

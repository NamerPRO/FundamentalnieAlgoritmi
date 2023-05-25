#ifndef STANDARD_PIPELINE_INTERPRITER_COMMANDS
#define STANDARD_PIPELINE_INTERPRITER_COMMANDS

#include <chrono>
#include <ctime>
#include <stdexcept>
#include <string>
#include <vector>

#include "../interpritator_manager/interpritator.h"
#include "./pipeline_command_invoker.h"
#include "./pipeline_base.h"
#include "./commands_creator.h"
#include "../flyweight_string/string_manager.h"

namespace npipeline {

    class add_pool final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        nmemory::memory * _allocator;
        
        std::string _pool_name;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new add_pool(dbase, dbase_with_developer_login_key, args[1], allocator);
            }

        };

        explicit add_pool(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            nmemory::memory * allocator
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _allocator(allocator) {}

        void execute() override;

        ~add_pool() = default;

    };

    class remove_pool final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        std::string _pool_name;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new remove_pool(dbase, dbase_with_developer_login_key, args[1]);
            }

        };

        explicit remove_pool(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)) {}

        void execute() override;

        ~remove_pool() = default;

    };

    class add_scheme final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        nmemory::memory * _allocator;
        
        std::string _pool_name;
        std::string _scheme_name;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new add_scheme(dbase, dbase_with_developer_login_key, args[1], args[2], allocator);
            }

        };

        explicit add_scheme(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            nmemory::memory * allocator
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _scheme_name(std::move(scheme_name)), _allocator(allocator) {}

        void execute() override;

        ~add_scheme() = default;

    };

    class remove_scheme final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        std::string _pool_name;
        std::string _scheme_name;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new remove_scheme(dbase, dbase_with_developer_login_key, args[1], args[2]);
            }

        };

        explicit remove_scheme(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _scheme_name(std::move(scheme_name)) {}

        void execute() override;

        ~remove_scheme() = default;

    };

    class add_collection final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        nmemory::memory * _allocator;
        
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new add_collection(dbase,dbase_with_developer_login_key, args[1], args[2], args[3], allocator);
            }

        };

        explicit add_collection(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            nmemory::memory * allocator
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)), _allocator(allocator) {}

        void execute() override;

        ~add_collection() = default;

    };

    class remove_collection final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new remove_collection(dbase, dbase_with_developer_login_key, args[1], args[2], args[3]);
            }

        };

        explicit remove_collection(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)) {}

        void execute() override;

        ~remove_collection() = default;

    };

    class add_or_update_note final : public ninterpritator::interpritator::command {

    private:

        nstring::string_factory & _factory = nstring::string_factory::get_instance();

        unsigned long long _date = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string const & _pool_name;
        std::string const & _scheme_name;
        std::string const & _collection_name;
        unsigned long _build_id;
        unsigned long _build_version;
        unsigned long _commit_hash;
        std::string const & _developer_login;
        std::string const & _developer_email;
        std::string const & _build_script_link;
        std::string const & _build_name;
        std::string const & _build_error_information;
        std::string const & _code_analysis_information;
        std::string const & _test_error_information;
        std::string const & _link_to_artifacts;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new add_or_update_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], std::stoul(args[6]), std::stoul(args[7]), std::stoul(args[8]), args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]);
            }

        };

        explicit add_or_update_note(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
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
            std::string & link_to_artifacts
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(_factory.get_string(std::move(pool_name))),
            _scheme_name(std::move(_factory.get_string(scheme_name))), _collection_name(_factory.get_string(std::move(collection_name))),
            _build_id(build_id), _build_version(build_version), _commit_hash(commit_hash),
            _developer_login(std::move(_factory.get_string(developer_login))), _developer_email(_factory.get_string(std::move(developer_email))),
            _build_script_link(_factory.get_string(std::move(build_script_link))), _build_name(_factory.get_string(std::move(build_name))),
            _build_error_information(_factory.get_string(std::move(build_error_information))), _code_analysis_information(_factory.get_string(std::move(code_analysis_information))),
            _test_error_information(_factory.get_string(std::move(test_error_information))), _link_to_artifacts(_factory.get_string(std::move(link_to_artifacts))) {}

        void execute() override;

        ~add_or_update_note() = default;

    };

    class read_note final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        std::string _key_type;
        unsigned long _build_id;
        unsigned long _build_version;
        std::string _developer_login;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                if (args[4] == "id+version") {
                    return new read_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], std::stoul(args[5]), std::stoul(args[6]), "");
                } else {
                    return new read_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], 0, 0, std::move(args[5]));
                }
            }

        };

        explicit read_note(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            std::string & key_type,
            unsigned long build_id,
            unsigned long build_version,
            std::string && developer_login
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version), _key_type(std::move(key_type)), _developer_login(std::move(developer_login)) {}

        void execute() override;

        ~read_note() = default;

    };

    class read_note_timed final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        std::string _key_type;
        unsigned long _build_id;
        unsigned long _build_version;
        std::string _developer_login;
        unsigned long long _time;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                if (args[4] == "id+version") {
                    return new read_note_timed(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], std::stoul(args[5]), std::stoul(args[6]), to_milliseconds(std::stoi(args[7]), std::stoi(args[8]), std::stoi(args[9]), std::stoi(args[10]), std::stoi(args[11]), std::stoi(args[12])), "");
                } else {
                    return new read_note_timed(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], 0, 0, to_milliseconds(std::stoi(args[6]), std::stoi(args[7]), std::stoi(args[8]), std::stoi(args[9]), std::stoi(args[10]), std::stoi(args[11])), std::move(args[5]));
                }
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
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            std::string & key_type,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long long time,
            std::string && developer_login
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version), _time(time), _key_type(std::move(key_type)), _developer_login(std::move(developer_login)) {}

        void execute() override;

        ~read_note_timed() = default;

    };

    class remove_note final : public ninterpritator::interpritator::command {

    private:

        unsigned long long _date = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        unsigned long _build_id;
        unsigned long _build_version;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new remove_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]));
            }

        };

        explicit remove_note(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version) {}

        void execute() override;

        ~remove_note() = default;

    };

    class read_in_range final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        std::string _key_type;
        unsigned long _build_id_start;
        unsigned long _build_version_start;
        unsigned long _build_id_end;
        unsigned long _build_version_end;
        std::string _developer_login_begin;
        std::string _developer_login_end;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                if (args[4] == "id+version") {
                    return new read_in_range(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], std::stoul(args[5]), std::stoul(args[6]), std::stoul(args[7]), std::stoul(args[8]), "", "");
                } else {
                    return new read_in_range(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], 0, 0, 0, 0, std::move(args[5]), std::move(args[6]));
                }
            }

        };

        explicit read_in_range(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            std::string & key_type,
            unsigned long build_id_start,
            unsigned long build_version_start,
            unsigned long build_id_end,
            unsigned long build_version_end,
            std::string && developer_login_begin,
            std::string && developer_login_end
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id_start(build_id_start), _build_version_start(build_version_start),
            _build_id_end(build_id_end), _build_version_end(build_version_end), _key_type(std::move(key_type)),
            _developer_login_begin(std::move(developer_login_begin)), _developer_login_end(std::move(developer_login_end)) {}

        void execute() override;

        ~read_in_range() = default;

    };

    class run_file final : public ninterpritator::interpritator::command {

    private:

        pipeline_base::data_base * _dbase;
        pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        nmemory::memory * _allocator;

        std::string _path_to_file;
        invoker * _cmd_invoker;

    public:

        class creator final : public command_creator {

        public:

            ninterpritator::interpritator::command * create_command(
                pipeline_base::data_base * dbase,
                pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator
            ) override {
                return new run_file(dbase, dbase_with_developer_login_key, args[1], allocator);
            }

        };

        explicit run_file(
            pipeline_base::data_base * dbase,
            pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & path_to_file,
            nmemory::memory * allocator
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _path_to_file(std::move(path_to_file)), _cmd_invoker(new invoker()), _allocator(allocator) {}

        void execute() override;

        ~run_file() {
            delete _cmd_invoker;
        }

    };
    
}

#endif /* STANDARD_PIPELINE_INTERPRITER_COMMANDS */

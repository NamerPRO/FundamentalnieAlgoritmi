#ifndef STANDARD_PIPELINE_INTERPRITER_COMMANDS
#define STANDARD_PIPELINE_INTERPRITER_COMMANDS

#include <chrono>
#include <ctime>
#include <stdexcept>
#include <string>
#include <vector>

#include "./command_invoker.h"

#include "../pipeline_manager/pipeline_base.h"

#include "./commands_creator.h"

#include "../flyweight_string/string_manager.h"

namespace npipeline_tools {

    class add_pool final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        nmemory::memory * _allocator;
        
        std::string _pool_name;

        std::string _uuid_as_string;

    private:

        explicit add_pool(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            nmemory::memory * allocator,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _allocator(allocator), _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new add_pool(dbase, dbase_with_developer_login_key, args[1], allocator, uuid_as_string);
            }

        };

        void execute() override;

        ~add_pool() = default;

    };

    class remove_pool final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        std::string _pool_name;

        std::string _uuid_as_string;

    private:

        explicit remove_pool(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new remove_pool(dbase, dbase_with_developer_login_key, args[1], uuid_as_string);
            }

        };

        void execute() override;

        ~remove_pool() = default;

    };

    class add_scheme final : command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        nmemory::memory * _allocator;
        
        std::string _pool_name;
        std::string _scheme_name;

        std::string _uuid_as_string;

    private:

        explicit add_scheme(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            nmemory::memory * allocator,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _scheme_name(std::move(scheme_name)), _allocator(allocator),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new add_scheme(dbase, dbase_with_developer_login_key, args[1], args[2], allocator, uuid_as_string);
            }

        };

        void execute() override;

        ~add_scheme() = default;

    };

    class remove_scheme final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        std::string _pool_name;
        std::string _scheme_name;

        std::string _uuid_as_string;

    private:

        explicit remove_scheme(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _scheme_name(std::move(scheme_name)),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new remove_scheme(dbase, dbase_with_developer_login_key, args[1], args[2], uuid_as_string);
            }

        };

        void execute() override;

        ~remove_scheme() = default;

    };

    class add_collection final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        nmemory::memory * _allocator;
        
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;

        std::string _uuid_as_string;

    private:

        explicit add_collection(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            nmemory::memory * allocator,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)), _allocator(allocator),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new add_collection(dbase,dbase_with_developer_login_key, args[1], args[2], args[3], allocator, uuid_as_string);
            }

        };

        void execute() override;

        ~add_collection() = default;

    };

    class remove_collection final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;
        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;

        std::string _uuid_as_string;

    private:

        explicit remove_collection(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _pool_name(std::move(pool_name)), _scheme_name(std::move(scheme_name)),
            _collection_name(std::move(collection_name)), _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new remove_collection(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], uuid_as_string);
            }

        };

        void execute() override;

        ~remove_collection() = default;

    };

    class add_or_update_note final : public command {

    private:

        nstring::string_factory & _factory = nstring::string_factory::get_instance();

        unsigned long long _date = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

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

        std::string _uuid_as_string;

    private:

        explicit add_or_update_note(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
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
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(_factory.get_string(std::move(pool_name))),
            _scheme_name(std::move(_factory.get_string(scheme_name))), _collection_name(_factory.get_string(std::move(collection_name))),
            _build_id(build_id), _build_version(build_version), _commit_hash(commit_hash),
            _developer_login(std::move(_factory.get_string(developer_login))), _developer_email(_factory.get_string(std::move(developer_email))),
            _build_script_link(_factory.get_string(std::move(build_script_link))), _build_name(_factory.get_string(std::move(build_name))),
            _build_error_information(_factory.get_string(std::move(build_error_information))), _code_analysis_information(_factory.get_string(std::move(code_analysis_information))),
            _test_error_information(_factory.get_string(std::move(test_error_information))), _link_to_artifacts(_factory.get_string(std::move(link_to_artifacts))),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new add_or_update_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], std::stoul(args[6]), std::stoul(args[7]), std::stoul(args[8]), args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16], uuid_as_string);
            }

        };

        void execute() override;

        ~add_or_update_note() = default;

    };

    class read_note final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        std::string _key_type;
        unsigned long _build_id;
        unsigned long _build_version;
        std::string _developer_login;

        std::string _uuid_as_string;

    private:

        explicit read_note(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            std::string & key_type,
            unsigned long build_id,
            unsigned long build_version,
            std::string && developer_login,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version), _key_type(std::move(key_type)), _developer_login(std::move(developer_login)),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                if (args[4] == "id+version") {
                    return new read_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], std::stoul(args[5]), std::stoul(args[6]), "", uuid_as_string);
                } else {
                    return new read_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], 0, 0, std::move(args[5]), uuid_as_string);
                }
            }

        };

        void execute() override;

        ~read_note() = default;

    };

    class read_note_timed final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        std::string _key_type;
        unsigned long _build_id;
        unsigned long _build_version;
        std::string _developer_login;
        unsigned long long _time;

        std::string _uuid_as_string;

    private:

        explicit read_note_timed(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            std::string & key_type,
            unsigned long build_id,
            unsigned long build_version,
            unsigned long long time,
            std::string && developer_login,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version), _time(time), _key_type(std::move(key_type)), _developer_login(std::move(developer_login)),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                if (args[4] == "id+version") {
                    return new read_note_timed(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], std::stoul(args[5]), std::stoul(args[6]), to_milliseconds(std::stoi(args[7]), std::stoi(args[8]), std::stoi(args[9]), std::stoi(args[10]), std::stoi(args[11]), std::stoi(args[12])), "", uuid_as_string);
                } else {
                    return new read_note_timed(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], 0, 0, to_milliseconds(std::stoi(args[6]), std::stoi(args[7]), std::stoi(args[8]), std::stoi(args[9]), std::stoi(args[10]), std::stoi(args[11])), std::move(args[5]), uuid_as_string);
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
                seconds,
                minutes,
                hours,
                day,
                month - 1,
                year - 1900
            };
            return std::mktime(&time) * 1000;
        }

        void execute() override;

        ~read_note_timed() = default;

    };

    class remove_note final : public command {

    private:

        unsigned long long _date = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        std::string _pool_name;
        std::string _scheme_name;
        std::string _collection_name;
        unsigned long _build_id;
        unsigned long _build_version;

        std::string _uuid_as_string;

    private:

        explicit remove_note(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            unsigned long build_id,
            unsigned long build_version,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id(build_id), _build_version(build_version), _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new remove_note(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), uuid_as_string);
            }

        };

        void execute() override;

        ~remove_note() = default;

    };

    class read_in_range final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

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

        std::string _uuid_as_string;

    private:

        explicit read_in_range(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & pool_name,
            std::string & scheme_name,
            std::string & collection_name,
            std::string & key_type,
            unsigned long build_id_start,
            unsigned long build_version_start,
            unsigned long build_id_end,
            unsigned long build_version_end,
            std::string && developer_login_begin,
            std::string && developer_login_end,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key), _pool_name(std::move(pool_name)),
            _scheme_name(std::move(scheme_name)), _collection_name(std::move(collection_name)),
            _build_id_start(build_id_start), _build_version_start(build_version_start),
            _build_id_end(build_id_end), _build_version_end(build_version_end), _key_type(std::move(key_type)),
            _developer_login_begin(std::move(developer_login_begin)), _developer_login_end(std::move(developer_login_end)),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                if (args[4] == "id+version") {
                    return new read_in_range(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], std::stoul(args[5]), std::stoul(args[6]), std::stoul(args[7]), std::stoul(args[8]), "", "", uuid_as_string);
                } else {
                    return new read_in_range(dbase, dbase_with_developer_login_key, args[1], args[2], args[3], args[4], 0, 0, 0, 0, std::move(args[5]), std::move(args[6]), uuid_as_string);
                }
            }

        };

        void execute() override;

        ~read_in_range() = default;

    };

    class run_file final : public command {

    private:

        npipeline::pipeline_base::data_base * _dbase;
        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        nmemory::memory * _allocator;

        std::string _path_to_file;
        invoker * _cmd_invoker;

        std::string _uuid_as_string;

    private:

        explicit run_file(
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            std::string & path_to_file,
            nmemory::memory * allocator,
            std::string & uuid_as_string
        ) : _dbase(dbase), _dbase_with_developer_login_key(dbase_with_developer_login_key),
            _path_to_file(std::move(path_to_file)), _cmd_invoker(new invoker()), _allocator(allocator),
            _uuid_as_string(uuid_as_string) {}

    public:

        class creator final : public command_creator {

        public:

            command * create_command(
                npipeline::pipeline_base::data_base * dbase,
                npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
                std::vector<std::string> & args,
                nmemory::memory * allocator,
                std::string & uuid_as_string
            ) override {
                return new run_file(dbase, dbase_with_developer_login_key, args[1], allocator, uuid_as_string);
            }

        };

        void execute() override;

        ~run_file() {
            delete _cmd_invoker;
        }

    };
    
}

#endif /* STANDARD_PIPELINE_INTERPRITER_COMMANDS */

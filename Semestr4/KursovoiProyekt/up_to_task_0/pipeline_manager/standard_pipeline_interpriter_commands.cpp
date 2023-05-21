#include "./standard_pipeline_interpriter_commands.h"
#include "./pipeline_base.h"
#include "pipeline_command_invoker.h"
#include "pipeline_interpriters.h"

namespace npipeline {

    void add_pool::execute() {
        pipeline_base::pool inserted_pool;
        _dbase->insert(_pool_name, std::move(inserted_pool));
    }

    void add_scheme::execute() {
        pipeline_base::scheme inserted_scheme;
        _dbase->find(_pool_name)
                 .insert(_scheme_name, std::move(inserted_scheme));
    }

    void add_collection::execute() {
        pipeline_base::collection inserted_collection;
        _dbase->find(_pool_name)
                 .find(_scheme_name)
                 .insert(_collection_name, std::move(inserted_collection));
    }

    void remove_pool::execute() {
        _dbase->remove(_pool_name);
    }

    void remove_scheme::execute() {
        _dbase->find(_pool_name)
                 .remove(_scheme_name);
    }

    void remove_collection::execute() {
        _dbase->find(_pool_name)
                 .find(_scheme_name)
                 .remove(_collection_name);
    }

    void add_or_update_note::execute() {
        pipeline_base::pipeline_passage struct_to_insert = {
            _build_id,
            _build_version,
            {
                _commit_hash,
                _developer_login,
                _developer_email
            },
            _build_script_link,
            _build_name,
            _build_error_information,
            _code_analysis_information,
            _test_error_information,
            _link_to_artifacts
        };
        _dbase->find(_pool_name)
                 .find(_scheme_name)
                 .find(_collection_name)
                 .insert({ _build_id, _build_version }, std::move(struct_to_insert));
    }

    void read_note::execute() {
        std::cout << _dbase->find(_pool_name).find(_scheme_name).find(_collection_name).find({ _build_id, _build_version }).to_string() << std::endl;
    }

    void remove_note::execute() {
        _dbase->find(_pool_name)
                 .find(_scheme_name)
                 .find(_collection_name)
                 .remove({ _build_id, _build_version });
    }

    void read_in_range::execute() {
        auto collection_tree = _dbase->find(_pool_name)
                                             .find(_scheme_name)
                                             .find(_collection_name);

        auto it_begin = collection_tree.infix_iterator_begin();
        while (std::get<1>(*it_begin).build_id != _build_id_start
                || std::get<1>(*it_begin).build_version != _build_version_start) {
            ++it_begin;
        }

        while (true) {
            std::cout << std::get<2>(*it_begin).to_string() << std::endl;
            if (std::get<1>(*it_begin).build_id == _build_id_end
                    && std::get<1>(*it_begin).build_version == _build_version_end) {
                break;
            }
            ++it_begin;
        }
    }

    void run_file::execute() {
        ninterpritator::interpritator * temp_interpriter = new pipeline_interpriter(_dbase, _path_to_file, _cmd_invoker);
        temp_interpriter->interpritate();
        delete temp_interpriter;
    }

    ninterpritator::interpritator::command * command_helper::get_cmd_ptr_by_name(
        pipeline_base::data_base * dbase,
        std::vector<std::string> & args,
        unsigned long long date,
        std::vector<std::string> & commands,
        invoker * cmd_invoker
    ) {
        if (args[0] == commands[0]) {
            return new add_pool(dbase, args[1], date);
        } else if (args[0] == commands[1]) {
            return new add_scheme(dbase, args[1], args[2], date);
        } else if (args[0] == commands[2]) {
            return new add_collection(dbase, args[1], args[2], args[3], date);
        } else if (args[0] == commands[3]) {
            return new add_or_update_note(dbase, args[1], args[2], args[3], std::stoul(args[6]), std::stoul(args[7]), std::stoul(args[8]), args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16], date);
        } else if (args[0] == commands[4]) {
            return new read_note(dbase, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), date);
        } else if (args[0] == commands[5]) {
            return new remove_note(dbase, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), date);
        } else if (args[0] == commands[6]) {
            return new remove_collection(dbase, args[1], args[2], args[3], date);
        } else if (args[0] == commands[7]) {
            return new remove_scheme(dbase, args[1], args[2], date);
        } else if (args[0] == commands[8]) {
            return new remove_pool(dbase, args[1], date);
        } else if (args[0] == commands[9]) {
            return new read_in_range(dbase, args[1], args[2], args[3], std::stoul(args[4]), std::stoul(args[5]), std::stoul(args[6]), std::stoul(args[7]), date);
        } else if (args[0] == commands[10]) {
            return new run_file(dbase, args[1], cmd_invoker, date);
        }
        throw std::runtime_error("Undefined command exception! Cannot find input command in list.");
    }

}
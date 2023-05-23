#include "./standard_pipeline_interpriter_commands.h"
#include "./pipeline_interpriters.h"
#include "pipeline_base.h"
#include <stdexcept>

namespace npipeline {

    pipeline_base::pipeline_passage pipeline_base::_empty_pipeline_passage = {
        .build_id = 0,
        .build_version = 0,
        .commit_information {
            .commit_hash = 0,
            .developer_login = "",
            .developer_email = ""
        },
        .build_script_link = "",
        .build_name = "",
        .build_error_information = "",
        .code_analysis_information = "",
        .test_error_information = "",
        .link_to_artifacts = ""
    };

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
        auto & found_collection = _dbase->find(_pool_name)
                                            .find(_scheme_name)
                                            .find(_collection_name);
        try {
            found_collection.find({ _build_id, _build_version })
                                .push_back({ _date, std::move(struct_to_insert) });
        } catch (std::runtime_error &) {
            found_collection.insert({ _build_id, _build_version }, { { _date, std::move(struct_to_insert) } });
        }
    }

    void read_note::execute() {
        auto & found_note = _dbase->find(_pool_name).find(_scheme_name).find(_collection_name).find({ _build_id, _build_version }).back().second;
        if (found_note == pipeline_base::_empty_pipeline_passage) {
            throw std::runtime_error("No such element exception! Could not find requested element by key.");
        }
        std::cout << found_note.to_string() << std::endl;
    }

    void read_note_timed::execute() {
        auto & found_note = _dbase->find(_pool_name).find(_scheme_name).find(_collection_name).find({ _build_id, _build_version });
        auto it_begin = found_note.begin();
        auto it_end = found_note.end();
        while (it_begin != it_end && it_begin->first <= _time) {
            ++it_begin;
        }
        if (it_begin == found_note.begin()) {
            throw std::runtime_error("No such element exception! Could not find requested element by key.");
        }
        --it_begin;
        if (it_begin->second == pipeline_base::_empty_pipeline_passage) {
            throw std::runtime_error("No such element exception! Could not find requested element by key.");
        }
        std::cout << (it_begin->second).to_string() << std::endl;
    }

    void remove_note::execute() {
        auto & found_data = _dbase->find(_pool_name)
                                    .find(_scheme_name)
                                    .find(_collection_name)
                                    .find({ _build_id, _build_version });
        
        found_data.push_back({ _date, std::move(pipeline_base::_empty_pipeline_passage) });
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
            if (std::get<2>(*it_begin).back().second != pipeline_base::_empty_pipeline_passage) {
                std::cout << std::get<2>(*it_begin).back().second.to_string() << std::endl;
            }
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

}
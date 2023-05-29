// #include "./standard_pipeline_interpriter_commands.h"
// #include "./pipeline_interpriters.h"
#include "./pipeline_commands.h"
#include "../pipeline_manager/pipeline_base.h"
#include "../storage_manager/data_storage.h"

npipeline::pipeline_base::pipeline_passage npipeline::pipeline_base::_empty_pipeline_passage = { 0, 0, { 0, "", "" }, "", "", "", "", "", "" };

namespace npipeline_tools {

    void add_pool::execute() {
        npipeline::pipeline_base::pool inserted_pool(_allocator);
        _dbase->insert(_pool_name, std::move(inserted_pool));

        npipeline::pipeline_base::pool_with_developer_login_key inserted_pool_with_developer_login_key(_allocator);
        _dbase_with_developer_login_key->insert(_pool_name, std::move(inserted_pool_with_developer_login_key));
    
        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Pool added successfully!";
    }

    void add_scheme::execute() {
        npipeline::pipeline_base::scheme inserted_scheme(_allocator);
        _dbase->find(_pool_name)
                 .insert(_scheme_name, std::move(inserted_scheme));

        npipeline::pipeline_base::scheme_with_developer_login_key inserted_scheme_with_developer_login_key(_allocator);
        _dbase_with_developer_login_key->find(_pool_name)
                                            .insert(_scheme_name, std::move(inserted_scheme_with_developer_login_key));
    
        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Scheme added successfully!";
    }

    void add_collection::execute() {
        npipeline::pipeline_base::collection inserted_collection(_allocator);
        _dbase->find(_pool_name)
                 .find(_scheme_name)
                 .insert(_collection_name, std::move(inserted_collection));
        
        npipeline::pipeline_base::collection_with_developer_login_key inserted_collection_with_developer_login_key(_allocator);
        _dbase_with_developer_login_key->find(_pool_name)
                                            .find(_scheme_name)
                                            .insert(_collection_name, std::move(inserted_collection_with_developer_login_key));
    
        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Collection added successfully!";
    }

    void remove_pool::execute() {
        _dbase->remove(_pool_name);
        _dbase_with_developer_login_key->remove(_pool_name);

        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Pool removed successfully!";
    }

    void remove_scheme::execute() {
        _dbase->find(_pool_name)
                    .remove(_scheme_name);
        
        _dbase_with_developer_login_key->find(_pool_name)
                                            .remove(_scheme_name);

        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Scheme removed successfully!";
    }

    void remove_collection::execute() {
        _dbase->find(_pool_name)
                    .find(_scheme_name)
                    .remove(_collection_name);
        
        _dbase_with_developer_login_key->find(_pool_name)
                                            .find(_scheme_name)
                                            .remove(_collection_name);

        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Collection removed successfully!";
    }

    void add_or_update_note::execute() {
        npipeline::pipeline_base::pipeline_passage struct_to_insert = {
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

        auto & found_collection_with_developer_login_key = _dbase_with_developer_login_key->find(_pool_name)
                                                                                                .find(_scheme_name)
                                                                                                .find(_collection_name);
        
        bool throw_exception = false;

        try {
            auto & found_list = found_collection.find({ _build_id, _build_version });
            
            std::string const & previous_developer_login = found_list.back().second.commit_information.developer_login;
            
            if (previous_developer_login == _developer_login) {
                found_list.push_back({ _date, std::move(struct_to_insert) });
            } else {
                throw_exception = true;
            }
        } catch (std::runtime_error &) {
            found_collection.insert({ _build_id, _build_version }, { { _date, std::move(struct_to_insert) } });
            auto & reference_to_inserted_list = found_collection.find({ _build_id, _build_version });
            found_collection_with_developer_login_key.insert(_developer_login, std::move(&reference_to_inserted_list));
        }

        if (throw_exception) {
            throw std::runtime_error("Developer login changed, but it must be immutable due to it is a possible key.");
        }

        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Note was successfully added/updated!";
    }

    void read_note::execute() {
        auto & found_note = (_key_type == "id+version") ? _dbase->find(_pool_name).find(_scheme_name).find(_collection_name).find({ _build_id, _build_version }).back().second : _dbase_with_developer_login_key->find(_pool_name).find(_scheme_name).find(_collection_name).find(_developer_login)->back().second;
        if (found_note == npipeline::pipeline_base::_empty_pipeline_passage) {
            throw std::runtime_error("No such element exception! Could not find requested element by key.");
        }
        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = found_note.to_string();
    }

    void read_note_timed::execute() {
        auto & found_note = (_key_type == "id+version") ? _dbase->find(_pool_name).find(_scheme_name).find(_collection_name).find({ _build_id, _build_version }) : *_dbase_with_developer_login_key->find(_pool_name).find(_scheme_name).find(_collection_name).find(_developer_login);
        auto it_begin = found_note.begin();
        auto it_end = found_note.end();
        while (it_begin != it_end && it_begin->first <= _time) {
            ++it_begin;
        }
        if (it_begin == found_note.begin()) {
            throw std::runtime_error("No such element exception! Could not find requested element by key.");
        }
        --it_begin;
        if (it_begin->second == npipeline::pipeline_base::_empty_pipeline_passage) {
            throw std::runtime_error("No such element exception! Could not find requested element by key.");
        }
        std::cout << (it_begin->second).to_string() << std::endl;
    }

    void remove_note::execute() {
        auto & found_data = _dbase->find(_pool_name)
                                    .find(_scheme_name)
                                    .find(_collection_name)
                                    .find({ _build_id, _build_version });

        if (found_data.back().second == npipeline::pipeline_base::_empty_pipeline_passage) {
            throw std::runtime_error("No such element exception! Cannot delete an element that does not exist.");
        }

        found_data.push_back({ _date, std::move(npipeline::pipeline_base::_empty_pipeline_passage) });

        storage & response_storage = storage::get_instanse();
        response_storage.get_responses()[_uuid_as_string] = "Note removed successfully!";
    }

    void read_in_range::execute() {
        if (_key_type == "id+version") {
            auto & collection_tree = _dbase->find(_pool_name)
                                            .find(_scheme_name)
                                            .find(_collection_name);

            auto it_begin = collection_tree.infix_iterator_begin();
            while (std::get<1>(*it_begin).build_id != _build_id_start || std::get<1>(*it_begin).build_version != _build_version_start) {
                ++it_begin;
            }

            while (true) {
                if (std::get<2>(*it_begin).back().second != npipeline::pipeline_base::_empty_pipeline_passage) {
                    storage & response_storage = storage::get_instanse();
                    response_storage.get_responses()[_uuid_as_string] += std::get<2>(*it_begin).back().second.to_string() + '\n';
                }
                if (std::get<1>(*it_begin).build_id == _build_id_end && std::get<1>(*it_begin).build_version == _build_version_end) {
                    break;
                }
                ++it_begin;
            }
        } else {
            auto & collection_tree = _dbase_with_developer_login_key->find(_pool_name)
                                                                        .find(_scheme_name)
                                                                        .find(_collection_name);

            auto it_begin = collection_tree.infix_iterator_begin();
            while (std::get<1>(*it_begin) != _developer_login_begin) {
                ++it_begin;
            }

            while (true) {
                if (std::get<2>(*it_begin)->back().second != npipeline::pipeline_base::_empty_pipeline_passage) {
                    storage & response_storage = storage::get_instanse();
                    response_storage.get_responses()[_uuid_as_string] += std::get<2>(*it_begin)->back().second.to_string() + '\n';
                }
                if (std::get<1>(*it_begin) == _developer_login_end) {
                    break;
                }
                ++it_begin;
            }
        }
    }
    
    void run_file::execute() {
        // ninterpritator::interpritator * temp_interpriter = new pipeline_interpriter(_dbase, _dbase_with_developer_login_key, _path_to_file, _cmd_invoker, _allocator);
        // temp_interpriter->interpritate();
        // delete temp_interpriter;
    }

}
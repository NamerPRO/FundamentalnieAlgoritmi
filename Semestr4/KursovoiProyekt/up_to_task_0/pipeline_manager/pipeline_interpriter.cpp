#include "pipeline.h"
#include <ios>
#include <string>

namespace npipeline {

    void pipeline::pipeline_interpriter::interpritate_file(
        std::string file_to_interpritate_path
    ) {
        std::ifstream file(file_to_interpritate_path);
        if (!file.is_open()) {
            throw std::runtime_error("File to interpritate cannot be opened! Does it exist at given path?");
        }
        std::string line;
        while (!file.eof()) {
            std::getline(file, line);
            std::vector<std::string> data;
            boost::split(data, line, boost::is_any_of(":"));
            for (int i = 0; i < _actions.size(); ++i) {
                if (_commands[i] == data[0]) {
                    _actions[i](_dbase, data);
                }
            }
        }
    }

    void pipeline::pipeline_interpriter::add_pool(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {
        boost::algorithm::trim(data[1]);
        pool * inserted_pool_ptr = new navl::avl_tree<std::string, scheme *, pipeline_comporators::standard_string_comporator>;
        dbase->insert(data[1], std::move(inserted_pool_ptr));
    }

    void pipeline::pipeline_interpriter::add_scheme(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {
        boost::algorithm::trim(data[1]);
        std::vector<std::string> path_to_scheme;
        boost::split(path_to_scheme, data[1], boost::is_any_of("/"));
        scheme * inserted_scheme_ptr = new navl::avl_tree<std::string, collection *, pipeline_comporators::standard_string_comporator>;
        dbase->find(path_to_scheme[0])
                ->insert(path_to_scheme[1], std::move(inserted_scheme_ptr));
    }

    void pipeline::pipeline_interpriter::add_collection(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {
        boost::algorithm::trim(data[1]);
        std::vector<std::string> path_to_collection;
        boost::split(path_to_collection, data[1], boost::is_any_of("/"));
        collection * inserted_collection_ptr = new navl::avl_tree<pipeline_key, pipeline_passage *, pipeline_comporators::standard_pipeline_comporator>;
        dbase->find(path_to_collection[0])
                ->find(path_to_collection[1])
                ->insert(path_to_collection[2], std::move(inserted_collection_ptr));
    }

    void pipeline::pipeline_interpriter::remove_pool(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {

    }

    void pipeline::pipeline_interpriter::remove_scheme(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {

    }

    void pipeline::pipeline_interpriter::remove_collection(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {
        boost::algorithm::trim(data[1]);
        std::vector<std::string> path_to_collection;
        boost::split(path_to_collection, data[1], boost::is_any_of("/"));
        pipeline::collection * deleted_collection = dbase->find(path_to_collection[0])
                                                            ->find(path_to_collection[1])
                                                            ->remove(path_to_collection[2]);
    }

    void pipeline::pipeline_interpriter::add_or_update_note(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {

    }

    void pipeline::pipeline_interpriter::read_note(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {

    }

    void pipeline::pipeline_interpriter::remove_note(
        pipeline::data_base * dbase,
        std::vector<std::string> & data
    ) {

    }

}
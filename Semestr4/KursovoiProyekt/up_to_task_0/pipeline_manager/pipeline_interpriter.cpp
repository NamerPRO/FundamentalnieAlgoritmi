#include "pipeline.h"

namespace npipeline {

    pipeline::tree_type pipeline::pipeline_interpriter::_content_type = _content_type;

    // Assuming that file has valid syntax
    void pipeline::pipeline_interpriter::interpritate_file(
        std::string file_to_interpritate_path

    ) {
        std::ifstream file(file_to_interpritate_path);
        if (!file.is_open()) {
            throw std::runtime_error("File to interpritate cannot be opened! Does it exist at given path?");
        }
        std::string token;
        std::vector<std::string> tokens_list;
        char symbol;
        bool allow_spaces = false;
        while (file >> std::noskipws >> symbol) {
            if (symbol == ' ' && !allow_spaces) {
                continue;
            }

            switch (symbol) {
            
            case '&':
                allow_spaces = true;
                tokens_list.push_back(std::move(token));
                break;

            case ':':
            case '/':
            case ',':
            case '\n':
            case EOF:
                allow_spaces = false;
                tokens_list.push_back(std::move(token));
                break;

            default:
                token.push_back(symbol);
                break;

            }

            if (symbol == '\n' || symbol == EOF) {
                for (int i = 0; i < _actions.size(); ++i) {
                    if (_commands[i] == tokens_list[0]) {
                        _actions[i](_dbase, tokens_list);
                    }
                }
                tokens_list.clear();
            }
        }
        file.close();
    }

    void pipeline::pipeline_interpriter::add_pool(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        pool inserted_pool;
        dbase->insert(data[1], std::move(inserted_pool));
    }

    void pipeline::pipeline_interpriter::add_scheme(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        scheme inserted_scheme;
        dbase->find(data[1])
                .insert(data[2], std::move(inserted_scheme));
    }

    void pipeline::pipeline_interpriter::add_collection(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        collection inserted_collection;
        dbase->find(data[1])
                .find(data[2])
                .insert(data[3], std::move(inserted_collection));
    }

    void pipeline::pipeline_interpriter::remove_pool(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        dbase->remove(data[1]);
    }

    void pipeline::pipeline_interpriter::remove_scheme(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        dbase->find(data[1])
                .remove(data[2]);
    }

    void pipeline::pipeline_interpriter::remove_collection(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        dbase->find(data[1])
                .find(data[2])
                .remove(data[3]);
    }

    void pipeline::pipeline_interpriter::add_or_update_note(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        pipeline::pipeline_passage struct_to_insert = {
            std::stoul(data[6]),
            std::stoul(data[7]),
            {
                std::stoul(data[8]),
                data[9],
                data[10]
            },
            data[11],
            data[12],
            data[13],
            data[14],
            data[15],
            data[16]
        };
        dbase->find(data[1])
                .find(data[2])
                .find(data[3])
                .insert({ struct_to_insert.build_id, struct_to_insert.build_version }, std::move(struct_to_insert));
    }

    void pipeline::pipeline_interpriter::read_note(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        std::cout << dbase->find(data[1]).find(data[2]).find(data[3]).find({ std::stoul(data[4]), std::stoul(data[5]) }).to_string() << std::endl;
    }

    void pipeline::pipeline_interpriter::remove_note(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        dbase->find(data[1])
                .find(data[2])
                .find(data[3])
                .remove({ std::stoul(data[4]), std::stoul(data[5]) });
    }

    void pipeline::pipeline_interpriter::read_in_range(
        pipeline::data_base * dbase,
        std::vector<std::string> const & data
    ) {
        auto collection_tree = dbase->find(data[1])
                                        .find(data[2])
                                        .find(data[3]);

        std::pair<unsigned int, unsigned int> from_key = { std::stoul(data[4]), std::stoul(data[5]) };
        std::pair<unsigned int, unsigned int> to_key = { std::stoul(data[6]), std::stoul(data[7]) };
        
        auto it_begin = collection_tree.infix_iterator_begin();
        while (std::get<1>(*it_begin).build_id != from_key.first
                || std::get<1>(*it_begin).build_version != from_key.second) {
            ++it_begin;
        }

        while (true) {
            std::cout << std::get<2>(*it_begin).to_string() << std::endl;
            if (std::get<1>(*it_begin).build_id == to_key.first
                    && std::get<1>(*it_begin).build_version == to_key.second) {
                break;
            }
            ++it_begin;
        }
    }

}
#ifndef PIPELINE_DATA_SAVE
#define PIPELINE_DATA_SAVE

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <fstream>
#include <type_traits>
#include <variant>
#include <vector>

#include "./pipeline_base.h"

#include "../flyweight_string/string_manager.h"

#include "../nbtree/btree.h"
#include "../nbst/binary_search_tree.h"
#include "../navl/avl_tree.h"

namespace npipeline_tools {

    class data_save final {

    private:

        std::string const _path;

        npipeline::pipeline_base::data_base * _dbase;

        npipeline::pipeline_base::data_base_with_developer_login_key * _dbase_with_developer_login_key;

        npipeline::pipeline_base::tree_type _main_tree_type;

    public:

        data_save(
            std::string && path,
            npipeline::pipeline_base::data_base * dbase,
            npipeline::pipeline_base::data_base_with_developer_login_key * dbase_with_developer_login_key,
            npipeline::pipeline_base::tree_type main_tree_type
        ) : _path(std::move(path)), _dbase(dbase), _main_tree_type(main_tree_type),
            _dbase_with_developer_login_key(dbase_with_developer_login_key) {}

    private:

        template<typename iterator_type>
        void save_tree(
            std::ofstream & out,
            niterators::iterator<iterator_type, std::string, npipeline::pipeline_base::pool> & it_begin_pool,
            niterators::iterator<iterator_type, std::string, npipeline::pipeline_base::pool> & it_end_pool
        ) {
            while (reinterpret_cast<iterator_type &>(it_begin_pool) != reinterpret_cast<iterator_type &>(it_end_pool)) {
                
                out.write("#", sizeof(char));

                // Saving pool key:
                std::string const & pool_name = std::get<1>(*it_begin_pool);
                unsigned int pool_name_size = pool_name.size();
                out.write(reinterpret_cast<char const *>(&pool_name_size), sizeof(unsigned int));
                out.write(pool_name.c_str(), sizeof(char) * pool_name_size);
                
                auto it_begin_scheme = std::get<2>(*it_begin_pool).prefix_iterator_begin();
                auto it_end_scheme = std::get<2>(*it_begin_pool).prefix_iterator_end();
                
                while (it_begin_scheme != it_end_scheme) {

                    out.write("^", sizeof(char));

                    // Saving scheme key:
                    std::string const & scheme_name = std::get<1>(*it_begin_scheme);
                    unsigned int scheme_name_size = scheme_name.size();
                    out.write(reinterpret_cast<char const *>(&scheme_name_size), sizeof(unsigned int));
                    out.write(scheme_name.c_str(), sizeof(char) * scheme_name_size);
                
                    auto it_begin_collection = std::get<2>(*it_begin_scheme).prefix_iterator_begin();
                    auto it_end_collection = std::get<2>(*it_begin_scheme).prefix_iterator_end();
                    
                    while (it_begin_collection != it_end_collection) {

                        out.write("&", sizeof(char));
                        
                        // Saving collection key:
                        std::string const & collection_name = std::get<1>(*it_begin_collection);
                        unsigned int collection_name_size = collection_name.size();
                        out.write(reinterpret_cast<char const *>(&collection_name_size), sizeof(unsigned int));
                        out.write(collection_name.c_str(), sizeof(char) * collection_name_size);
                        
                        auto it_begin_data = std::get<2>(*it_begin_collection).prefix_iterator_begin();
                        auto it_end_data = std::get<2>(*it_begin_collection).prefix_iterator_end();

                        while (it_begin_data != it_end_data) {

                            out.write("!", sizeof(char));

                            // Saving data key & value (with node history):
                            auto it_begin_list = std::get<2>(*it_begin_data).begin();
                            auto it_end_list = std::get<2>(*it_begin_data).end();

                            while (it_begin_list != it_end_list) {

                                out.write("@", sizeof(char));

                                unsigned long long time = it_begin_list->first;
                                out.write(reinterpret_cast<char const *>(&time), sizeof(unsigned long long));

                                auto const & data = it_begin_list->second;
                                
                                out.write(reinterpret_cast<char const *>(&data.build_id), sizeof(unsigned long));
                                
                                out.write(reinterpret_cast<char const *>(&data.build_version), sizeof(unsigned long));
                                
                                out.write(reinterpret_cast<char const *>(&data.commit_information.commit_hash), sizeof(unsigned long));
                                
                                unsigned int developer_login_size = data.commit_information.developer_login.size();
                                out.write(reinterpret_cast<char const *>(&developer_login_size), sizeof(unsigned int));
                                out.write(data.commit_information.developer_login.c_str(), sizeof(char) * developer_login_size);
                                
                                unsigned int developer_email_size = data.commit_information.developer_email.size();
                                out.write(reinterpret_cast<char const *>(&developer_email_size), sizeof(unsigned int));
                                out.write(data.commit_information.developer_email.c_str(), sizeof(char) * developer_email_size);
                                
                                unsigned int build_script_link_size = data.build_script_link.size();
                                out.write(reinterpret_cast<char const *>(&build_script_link_size), sizeof(unsigned int));
                                out.write(data.build_script_link.c_str(), sizeof(char) * build_script_link_size);

                                unsigned int build_name_size = data.build_name.size();
                                out.write(reinterpret_cast<char const *>(&build_name_size), sizeof(unsigned int));
                                out.write(data.build_name.c_str(), sizeof(char) * build_name_size);

                                unsigned int build_error_information_size = data.build_error_information.size();
                                out.write(reinterpret_cast<char const *>(&build_error_information_size), sizeof(unsigned int));
                                out.write(data.build_error_information.c_str(), sizeof(char) * build_error_information_size);

                                unsigned int code_analysis_information_size = data.code_analysis_information.size();
                                out.write(reinterpret_cast<char const *>(&code_analysis_information_size), sizeof(unsigned int));
                                out.write(data.code_analysis_information.c_str(), sizeof(char) * code_analysis_information_size);
                                
                                unsigned int test_error_information_size = data.test_error_information.size();
                                out.write(reinterpret_cast<char const *>(&test_error_information_size), sizeof(unsigned int));
                                out.write(data.test_error_information.c_str(), sizeof(char) * data.test_error_information.size());
                                
                                unsigned int link_to_artifacts_size = data.link_to_artifacts.size();
                                out.write(reinterpret_cast<char const *>(&link_to_artifacts_size), sizeof(unsigned int));
                                out.write(data.link_to_artifacts.c_str(), sizeof(char) * link_to_artifacts_size);

                                ++it_begin_list;
                            }

                            ++it_begin_data;
                        }

                        ++it_begin_collection;
                    }

                    ++it_begin_scheme;
                }

                ++it_begin_pool;
            }

            out.write("%", sizeof(char));
        }

        void read_string_from_binary_file(
            std::ifstream & in,
            char ** string_buffer
        ) {
            static unsigned int buffer_length = 100;

            unsigned int string_length;
            in.read(reinterpret_cast<char *>(&string_length), sizeof(unsigned int));

            while (buffer_length + 1 < string_length) {
                char * possible_buffer = (char *)realloc(*string_buffer, sizeof(char) * buffer_length * 2);
                if (possible_buffer == NULL) {
                    free(*string_buffer);
                    throw std::runtime_error("Memory allocate exception! Cannot reallocate memory while restoring tree.");
                }
                buffer_length *= 2;
                *string_buffer = possible_buffer;
            }

            in.read(*string_buffer, sizeof(char) * string_length);

            (*string_buffer)[string_length] = '\0';
        }

        void save_btree(
            std::ofstream & out,
            nbtree::btree<std::string, npipeline::pipeline_base::pool, npipeline::pipeline_base::pipeline_comporators::standard_string_comporator> * dbase
        ) {
            throw std::runtime_error("Prevented an attempt to use unimplemented function save_btree...");
        }

    public:

        void save() {
            std::ofstream out(_path, std::ios::binary);

            if (!out.is_open()) {
                throw std::runtime_error("File interract exception! Cannot open a file. Does it exist on given path?");
            }
            
            switch (_main_tree_type) {

            case npipeline::pipeline_base::tree_type::avl:
            case npipeline::pipeline_base::tree_type::splay: {
                auto binary_tree_ptr = reinterpret_cast<nbst::binary_search_tree<std::string, npipeline::pipeline_base::pool, npipeline::pipeline_base::pipeline_comporators::standard_string_comporator> *>(_dbase);
                auto it_pool_begin = binary_tree_ptr->prefix_iterator_begin();
                auto it_pool_end = binary_tree_ptr->prefix_iterator_end();
                save_tree<nbst::binary_search_tree<std::string, npipeline::pipeline_base::pool, npipeline::pipeline_base::pipeline_comporators::standard_string_comporator>::prefix_iterator>(
                    out,
                    it_pool_begin,
                    it_pool_end
                );
                break;
            }

            case npipeline::pipeline_base::tree_type::btree: {
                auto btree_ptr = reinterpret_cast<nbtree::btree<std::string, npipeline::pipeline_base::pool, npipeline::pipeline_base::pipeline_comporators::standard_string_comporator> *>(_dbase);
                auto it_pool_begin = btree_ptr->prefix_iterator_begin();
                auto it_pool_end = btree_ptr->prefix_iterator_end();
                save_tree<nbtree::btree<std::string, npipeline::pipeline_base::pool, npipeline::pipeline_base::pipeline_comporators::standard_string_comporator>::prefix_iterator>(
                    out,
                    it_pool_begin,
                    it_pool_end
                );
                break;
            }

            }

            out.close();
        }

        void restore() {
            std::ifstream in(_path, std::ios::binary);
            if (!in.is_open()) {
                return;
            }

            char char_token;
            unsigned long ulong_token;
            char * string_buffer;
            std::vector<std::string> path(3);
            nstring::string_factory & factory = nstring::string_factory::get_instance();
            if ((string_buffer = (char *)malloc(sizeof(char) * 100)) == NULL) {
                throw std::runtime_error("Memory allocate exception! Cannot allocate memory while restoring tree.");
            }

            in.read(&char_token, sizeof(char));

            // If char_token = % tree data is restored, else continue restoring
            while (char_token != '%') {
                
                switch (char_token) {

                // Read new pool.
                case '#': {
                    read_string_from_binary_file(in, &string_buffer);
                    path[0] = string_buffer;
                    npipeline::pipeline_base::pool pool_to_insert;
                    _dbase->insert(path[0], std::move(pool_to_insert));
                    npipeline::pipeline_base::pool_with_developer_login_key pool_with_developer_login_key_to_insert;
                    _dbase_with_developer_login_key->insert(path[0], std::move(pool_with_developer_login_key_to_insert));
                    in.read(&char_token, sizeof(char));
                    break;
                }

                // Read new scheme.
                case '^': {
                    read_string_from_binary_file(in, &string_buffer);
                    path[1] = string_buffer;
                    npipeline::pipeline_base::scheme scheme_to_insert;
                    _dbase->find(path[0]).insert(path[1], std::move(scheme_to_insert));
                    npipeline::pipeline_base::scheme_with_developer_login_key scheme_to_insert_with_developer_value_key;
                    _dbase_with_developer_login_key->find(path[0]).insert(path[1], std::move(scheme_to_insert_with_developer_value_key));
                    in.read(&char_token, sizeof(char));
                    break;
                }

                // Read new collection.
                case '&': {
                    read_string_from_binary_file(in, &string_buffer);
                    path[2] = string_buffer;
                    npipeline::pipeline_base::collection collection_to_insert;
                    _dbase->find(path[0]).find(path[1]).insert(path[2], std::move(collection_to_insert));
                    npipeline::pipeline_base::collection_with_developer_login_key collection_to_insert_with_developer_login_key;
                    _dbase_with_developer_login_key->find(path[0]).find(path[1]).insert(path[2], std::move(collection_to_insert_with_developer_login_key));
                    in.read(&char_token, sizeof(char));
                    break;
                }

                // Read new note data
                case '!': {

                    in.read(&char_token, sizeof(char));

                    bool insert_node_flag = true;
                    unsigned long saved_build_id = 0;
                    unsigned long saved_build_version = 0;

                    // If char_token = @ we have a data to read
                    while (char_token == '@') {
                        std::vector<std::variant<std::string, unsigned long>> data_as_vector;

                        // Get time command was processed
                        unsigned long long time;
                        in.read(reinterpret_cast<char *>(&time), sizeof(unsigned long long));

                        // Getting build id, build version, commit hash
                        for (int i = 0; i < 3; ++i) {
                            in.read(reinterpret_cast<char *>(&ulong_token), sizeof(unsigned long));
                            data_as_vector.push_back(ulong_token);
                        }

                        // Getting other fileds
                        for (int i = 0; i < 8; ++i) {
                            read_string_from_binary_file(in, &string_buffer);
                            data_as_vector.push_back(string_buffer);
                        }

                        npipeline::pipeline_base::pipeline_passage data = {
                            .build_id = std::get<unsigned long>(data_as_vector[0]),
                            .build_version = std::get<unsigned long>(data_as_vector[1]),
                            .commit_information = {
                                .commit_hash = std::get<unsigned long>(data_as_vector[2]),
                                .developer_login = factory.get_string(std::move(std::get<std::string>(data_as_vector[3]))),
                                .developer_email = factory.get_string(std::move(std::get<std::string>(data_as_vector[4])))
                            },
                            .build_script_link = factory.get_string(std::move(std::get<std::string>(data_as_vector[5]))),
                            .build_name = factory.get_string(std::move(std::get<std::string>(data_as_vector[6]))),
                            .build_error_information = factory.get_string(std::move(std::get<std::string>(data_as_vector[7]))),
                            .code_analysis_information = factory.get_string(std::move(std::get<std::string>(data_as_vector[8]))),
                            .test_error_information = factory.get_string(std::move(std::get<std::string>(data_as_vector[9]))),
                            .link_to_artifacts = factory.get_string(std::move(std::get<std::string>(data_as_vector[10])))
                        };

                        auto & found_collection = _dbase->find(path[0]).find(path[1]).find(path[2]);
                        
                        if (insert_node_flag) {
                            found_collection.insert({ data.build_id, data.build_version }, { { time, std::move(data) } });
                            auto & reference_inserted_list = found_collection.find({ data.build_id, data.build_version });
                            _dbase_with_developer_login_key->find(path[0]).find(path[1]).find(path[2]).insert(data.commit_information.developer_login, std::move(&reference_inserted_list));
                            insert_node_flag = false;
                            saved_build_id = data.build_id;
                            saved_build_version = data.build_version;
                        } else {
                            found_collection.find({ saved_build_id, saved_build_version })
                                                .push_back({ time, std::move(data) });
                        }

                        in.read(&char_token, sizeof(char));
                    }

                    break;
                }

                }
            }

            in.close();
        }

    };

}

#endif /* PIPELINE_DATA_SAVE */

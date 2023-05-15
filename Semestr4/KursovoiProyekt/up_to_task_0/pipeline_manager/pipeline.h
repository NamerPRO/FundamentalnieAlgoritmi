#ifndef PIPELINE
#define PIPELINE

#include <any>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "../nbst/associative_container.h"
#include "../navl/avl_tree.h"

namespace npipeline {

    class pipeline : private ntools::nmalloc {

    friend class pipeline_interpriter;
    
    private:

        // Key and value implementation

        struct pipeline_key {

            unsigned int build_id;
            unsigned int build_version;

        };

        struct pipeline_passage {
            
            unsigned int build_id;
            unsigned int build_version;
            struct {

                unsigned int commit_hash;
                std::string developer_login;
                std::string developer_email;

            } commit_information;
            std::string build_script_link;
            std::string build_name;
            std::string build_error_information;
            std::string code_analysis_information;
            std::string test_error_information;
            std::string link_to_artifacts;

        };

        // ===

        // Pipeline comporators implementation

        class pipeline_comporators final {
            
        public:

            static bool standard_pipeline_comporator(
                const pipeline_key & lhs,
                const pipeline_key & rhs
            );

            static bool standard_string_comporator(
                const std::string & lhs,
                const std::string & rhs
            );

        };

        // ===

        using collection = ncontainer::associative_container<pipeline_key, pipeline_passage *, pipeline_comporators::standard_pipeline_comporator>;
        using scheme = ncontainer::associative_container<std::string, collection *, pipeline_comporators::standard_string_comporator>;
        using pool = ncontainer::associative_container<std::string, scheme *, pipeline_comporators::standard_string_comporator>;
        using data_base = ncontainer::associative_container<std::string, pool *, pipeline_comporators::standard_string_comporator>;

    public:

        enum class tree_type {
            avl
        };

    private:

        data_base * _dbase;

        tree_type _container_type;

        // Interpriter implementation

        class pipeline_interpriter final {

        private:

            pipeline::data_base * _dbase;
        
        private:

            static void add_pool(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void remove_pool(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void add_scheme(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void remove_scheme(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void add_collection(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void remove_collection(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void add_or_update_note(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void read_note(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            static void remove_note(
                pipeline::data_base * dbase,
                std::vector<std::string> & data
            );

            std::vector<std::string> _commands = { "ADD_POOL", "ADD_SCHEME", "ADD_COLLECTION", "ADD_OR_UPDATE_NOTE", "READ_NOTE", "REMOVE_NOTE", "REMOVE_COLLECTION", "REMOVE_SCHEME", "REMOVE_POOL", "READ_IN_RANGE" };
            std::vector<void (*)(pipeline::data_base * dbase, std::vector<std::string> &)> _actions = { add_pool, add_scheme, add_collection, add_or_update_note, read_note, remove_note, remove_collection, remove_scheme, remove_pool };

        public:

            explicit pipeline_interpriter(
                pipeline::data_base * dbase
            ) : _dbase(dbase) {};

            void interpritate_file(
                std::string file_to_interpritate_path
            );

        };
    
        // ===

        pipeline_interpriter * _interpriter;

    public:

        void run(
            std::string const & path_to_file
        ) {
            _interpriter->interpritate_file(path_to_file);
        }

    // public:

    //     pool * insert_pool(
    //         std::string const & pool_name
    //     ) {
    //         pool * inserted_pool_ptr = new navl::avl_tree<std::string, scheme *, pipeline_comporators::standard_string_comporator>;
    //         _dbase->insert(pool_name, std::move(inserted_pool_ptr));
    //         return inserted_pool_ptr;
    //     }

    //     void remove_pool(
    //         std::string const & pool_name
    //     ) {
    //         delete _dbase->remove(pool_name);
    //     }

    //     scheme * insert_scheme(
    //         std::string const & pool_name_where_to_insert_scheme,
    //         std::string const & scheme_name
    //     ) {
    //         scheme * inserted_scheme_ptr = new navl::avl_tree<std::string, collection *, pipeline_comporators::standard_string_comporator>;
    //         _dbase->find(pool_name_where_to_insert_scheme)
    //                 ->insert(scheme_name, std::move(inserted_scheme_ptr));
    //         return inserted_scheme_ptr;
    //     }

    //     void remove_scheme(
    //         std::string const & pool_name_where_to_remove_scheme,
    //         std::string const & scheme_name
    //     ) {
    //         delete _dbase->find(pool_name_where_to_remove_scheme)
    //                         ->remove(scheme_name);
    //     }

    //     collection * insert_collection(
    //         std::string const & pool_name_where_to_insert_collection,
    //         std::string const & scheme_name_where_to_insert_collection,
    //         std::string const & collection_name
    //     ) {
    //         collection * inserted_collection_ptr = new navl::avl_tree<pipeline_key, pipeline_passage *, pipeline_comporators::standard_pipeline_comporator>;
    //         _dbase->find(pool_name_where_to_insert_collection)
    //                 ->find(scheme_name_where_to_insert_collection)
    //                 ->insert(collection_name, std::move(inserted_collection_ptr));
    //         return inserted_collection_ptr;
    //     }

    //     void remove_collection(
    //         std::string const & pool_name_where_to_remove_collection,
    //         std::string const & scheme_name_where_to_remove_collection,
    //         std::string const & collection_name
    //     ) {
    //         delete _dbase->find(pool_name_where_to_remove_collection)
    //                         ->find(scheme_name_where_to_remove_collection)
    //                         ->remove(collection_name);
    //     }

        pipeline(
            tree_type type
        ) : pipeline(type, nullptr) {}

        pipeline(
            tree_type type,
            nmemory::memory * allocator
        ) : ntools::nmalloc(allocator), _container_type(type) {
            switch (_container_type) {
                
            case tree_type::avl:
                _dbase = new navl::avl_tree<std::string, pool *, pipeline_comporators::standard_string_comporator>;
                break;
            }

            _interpriter = new pipeline_interpriter(_dbase);
        }

    };

}

#endif /* PIPELINE */

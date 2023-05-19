#ifndef PIPELINE
#define PIPELINE

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../interpritator_manager/interpritator.h"
#include "../nbst/associative_container.h"
#include "../navl/avl_tree.h"

namespace npipeline {

   class pipeline : private ntools::nmalloc {

    friend class pipeline_interpriter;
    friend class user_interpriter;
    
    private:

        // Key and value implementation

        struct pipeline_key {

            unsigned long build_id;
            unsigned long build_version;

        };

        struct pipeline_passage {

        public:
            
            unsigned long build_id;
            unsigned long build_version;
            struct {

                unsigned long commit_hash;
                std::string developer_login;
                std::string developer_email;

            } commit_information;
            std::string build_script_link;
            std::string build_name;
            std::string build_error_information;
            std::string code_analysis_information;
            std::string test_error_information;
            std::string link_to_artifacts;

        public:

            std::string to_string() {
                return std::to_string(build_id) + '&' + std::to_string(build_version) +
                        '&' + std::to_string(commit_information.commit_hash) + '&' + commit_information.developer_login +
                        '&' + commit_information.developer_email + '&' + build_script_link + '&' + build_name + '&' + build_error_information +
                        '&' + code_analysis_information + '&' + test_error_information + '&' + link_to_artifacts;
            }

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

    public:

        enum class tree_type {
            avl
        };

        enum class interpriter_type {
            user_input_interpriter,
            file_input_interpriter
        };

    protected:

        using collection = navl::avl_tree<pipeline_key, pipeline_passage, pipeline_comporators::standard_pipeline_comporator>;
        using scheme = navl::avl_tree<std::string, collection, pipeline_comporators::standard_string_comporator>;
        using pool = navl::avl_tree<std::string, scheme, pipeline_comporators::standard_string_comporator>;
        using data_base = ncontainer::associative_container<std::string, pool, pipeline_comporators::standard_string_comporator>;

    private:

        data_base * _dbase;

        tree_type _container_type;

        // Interpriters implementation

        // Contract: Every pipeline interpriter must extend this
        class standard_pipeline_interpriter_kit {

        protected:

            static void add_pool(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void remove_pool(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void add_scheme(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void remove_scheme(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void add_collection(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void remove_collection(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void add_or_update_note(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void read_note(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void remove_note(
                data_base * dbase,
                std::vector<std::string> const & data
            );

            static void read_in_range(
                data_base * dbase,
                std::vector<std::string> const & data
            );

        };

        class pipeline_interpriter final : public ninterpritator::interpritator, private standard_pipeline_interpriter_kit {

        private:

            data_base * _dbase;

            std::string const & _file_to_interpritate_path;
        
        private:

            std::vector<std::string> _commands = { "ADD_POOL", "ADD_SCHEME", "ADD_COLLECTION", "ADD_OR_UPDATE_NOTE", "READ_NOTE", "REMOVE_NOTE", "REMOVE_COLLECTION", "REMOVE_SCHEME", "REMOVE_POOL", "READ_IN_RANGE" };
            std::vector<void (*)(data_base * dbase, std::vector<std::string> const &)> _actions = { add_pool, add_scheme, add_collection, add_or_update_note, read_note, remove_note, remove_collection, remove_scheme, remove_pool, read_in_range };

        public:

            explicit pipeline_interpriter(
                data_base * dbase,
                std::string const & file_to_interpritate_path
            ) : _dbase(dbase), _file_to_interpritate_path(file_to_interpritate_path) {}

            void interpritate() override;

            ~pipeline_interpriter() = default;

        };

        class user_interpriter final : public ninterpritator::interpritator, private standard_pipeline_interpriter_kit {
            
        private:

            data_base * _dbase;

        private:

            static void run_file(
                data_base * dbase,
                std::vector<std::string> const & data
            );

        private:

            std::vector<std::string> _commands = { "add pool", "add scheme", "add collection", "add or update note", "read note", "remove note", "remove collection", "remove scheme", "remove pool", "read in range", "run file" };
            std::vector<void (*)(data_base * dbase, std::vector<std::string> const &)> _actions = { add_pool, add_scheme, add_collection, add_or_update_note, read_note, remove_note, remove_collection, remove_scheme, remove_pool, read_in_range, run_file };

        public:

            explicit user_interpriter(
                data_base * dbase
            ) : _dbase(dbase) {}

            void interpritate() override;

            ~user_interpriter() = default;

        };
    
        // ===

        ninterpritator::interpritator * _interpriter;

    public:

        void run() {
            _interpriter->interpritate();
        }

        pipeline(
            tree_type content_type,
            interpriter_type interpriter_to_use,
            std::string const & path_to_file = "",
            nmemory::memory * allocator = nullptr
        ) : ntools::nmalloc(allocator) {
            _container_type = content_type;

            switch (_container_type) {
                
            case tree_type::avl:
                _dbase = new navl::avl_tree<std::string, pool, pipeline_comporators::standard_string_comporator>(allocator);
                break;
            
            }

            switch (interpriter_to_use) {

            case interpriter_type::file_input_interpriter:
                _interpriter = new pipeline_interpriter(_dbase, path_to_file);
                break;

            case interpriter_type::user_input_interpriter:
                _interpriter = new user_interpriter(_dbase);
                break;
            
            }
        }

        ~pipeline() {
            delete _interpriter;
            delete _dbase;
        }

    };

}

#endif /* PIPELINE */

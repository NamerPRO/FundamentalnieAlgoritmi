#ifndef PIPELINE_BASE
#define PIPELINE_BASE

#include <list>
#include <string>
#include <utility>

#include "../navl/avl_tree.h"

namespace npipeline {

    class pipeline_base {

        public:

            enum class tree_type {
                avl,
                splay,
                btree
            };

            enum class interpriter_type {
                user_input_interpriter,
                file_input_interpriter
            };

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
                    std::string const & developer_login;
                    std::string const & developer_email;

                } commit_information;
                std::string const & build_script_link;
                std::string const & build_name;
                std::string const & build_error_information;
                std::string const & code_analysis_information;
                std::string const & test_error_information;
                std::string const & link_to_artifacts;

            public:

                std::string to_string(
                    char sep = '&'
                ) {
                    return std::to_string(build_id) + sep + std::to_string(build_version) +
                            sep + std::to_string(commit_information.commit_hash) + sep + commit_information.developer_login +
                            sep + commit_information.developer_email + sep + build_script_link + sep + build_name + sep + build_error_information +
                            sep + code_analysis_information + sep + test_error_information + sep + link_to_artifacts;
                }

                bool operator==(
                    pipeline_passage const & rhs
                ) const {
                    if (build_id == rhs.build_id && build_version == rhs.build_version && commit_information.commit_hash == rhs.commit_information.commit_hash
                            && commit_information.developer_email == rhs.commit_information.developer_email && commit_information.developer_login == rhs.commit_information.developer_login
                                && build_script_link == rhs.build_script_link && build_name == rhs.build_name && build_error_information == rhs.build_error_information && code_analysis_information == rhs.code_analysis_information
                                    && test_error_information == rhs.test_error_information && link_to_artifacts == rhs.link_to_artifacts) {
                        return true;
                    }
                    return false;
                }

                bool operator!=(
                    pipeline_passage const & rhs
                ) const {
                    return !(*this == rhs);
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

            // Data base implementation

            // Search by { build_id, build_version }

            using collection = navl::avl_tree<pipeline_key, std::list<std::pair<unsigned long long, pipeline_passage>>, pipeline_comporators::standard_pipeline_comporator>;
            using scheme = navl::avl_tree<std::string, collection, pipeline_comporators::standard_string_comporator>;
            using pool = navl::avl_tree<std::string, scheme, pipeline_comporators::standard_string_comporator>;
            using data_base = ncontainer::associative_container<std::string, pool, pipeline_comporators::standard_string_comporator>;

            // ===

            // Search by developer login

            using collection_with_developer_login_key = navl::avl_tree<std::string, std::list<std::pair<unsigned long long, pipeline_passage>> *, pipeline_comporators::standard_string_comporator>;
            using scheme_with_developer_login_key = navl::avl_tree<std::string, collection_with_developer_login_key, pipeline_comporators::standard_string_comporator>;
            using pool_with_developer_login_key = navl::avl_tree<std::string, scheme_with_developer_login_key, pipeline_comporators::standard_string_comporator>;
            using data_base_with_developer_login_key = ncontainer::associative_container<std::string, pool_with_developer_login_key, pipeline_comporators::standard_string_comporator>;

            // ===

            // Empty pipeline passage implementation

            static pipeline_passage _empty_pipeline_passage;

            // ===

    };

}

#endif /* PIPELINE_BASE */

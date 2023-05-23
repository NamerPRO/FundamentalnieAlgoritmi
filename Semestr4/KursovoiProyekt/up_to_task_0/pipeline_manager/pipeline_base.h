#ifndef PIPELINE_BASE
#define PIPELINE_BASE

#include <list>
#include <string>

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

                bool operator==(
                    pipeline_passage & rhs
                ) {
                    if (build_id == rhs.build_id && build_version == rhs.build_version && commit_information.commit_hash == rhs.commit_information.commit_hash
                            && commit_information.developer_email == rhs.commit_information.developer_email && commit_information.developer_login == rhs.commit_information.developer_login
                                && build_script_link == rhs.build_script_link && build_name == rhs.build_name && build_error_information == rhs.build_error_information && code_analysis_information == rhs.code_analysis_information
                                    && test_error_information == rhs.test_error_information && link_to_artifacts == rhs.link_to_artifacts) {
                        return true;
                    }
                    return false;
                }

                bool operator!=(
                    pipeline_passage & rhs
                ) {
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

            using collection = navl::avl_tree<pipeline_key, std::list<std::pair<unsigned long long, pipeline_passage>>, pipeline_comporators::standard_pipeline_comporator>;
            using scheme = navl::avl_tree<std::string, collection, pipeline_comporators::standard_string_comporator>;
            using pool = navl::avl_tree<std::string, scheme, pipeline_comporators::standard_string_comporator>;
            using data_base = ncontainer::associative_container<std::string, pool, pipeline_comporators::standard_string_comporator>;

            // ===

            // Empty pipeline passage implementation

            static pipeline_passage _empty_pipeline_passage;

            // ===

    };

}

#endif /* PIPELINE_BASE */

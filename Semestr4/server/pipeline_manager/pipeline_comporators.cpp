#include "./pipeline_base.h"

namespace npipeline {

    bool pipeline_base::pipeline_comporators::standard_pipeline_comporator(
        pipeline_key const & lhs,
        pipeline_key const & rhs
    ) {
        if (lhs.build_id != rhs.build_id) {
            return lhs.build_id < rhs.build_id;
        }
        return lhs.build_version < rhs.build_version;
    }

    bool pipeline_base::pipeline_comporators::standard_string_comporator(
        std::string const & lhs,
        std::string const & rhs
    ) {
        for (int i = 0; i < lhs.size(); ++i) {
            if (lhs[i] == rhs[i]) {
                continue;
            }
            if (lhs[i] < rhs[i]) {
                return true;
            }
            return false;
        }
        return false;
    }

}
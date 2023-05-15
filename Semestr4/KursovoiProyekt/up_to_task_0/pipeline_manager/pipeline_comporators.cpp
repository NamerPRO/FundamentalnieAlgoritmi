#include "pipeline.h"

namespace npipeline {

    bool pipeline::pipeline_comporators::standard_pipeline_comporator(
        const pipeline_key & lhs,
        const pipeline_key & rhs
    ) {
        if (lhs.build_id != rhs.build_id) {
            return lhs.build_id < rhs.build_id;
        }
        return lhs.build_version < rhs.build_version;
    }

    bool pipeline::pipeline_comporators::standard_string_comporator(
        const std::string & lhs,
        const std::string & rhs
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
#ifndef PIPELINE_SERIALIZER
#define PIPELINE_SERIALIZER

#include <string>
#include <vector>

#include "../interpriter_manager/interpriter.h"

namespace npipeline_tools {

    class pipeline_serializer final {

    private:

        std::vector<std::string> const & _parsed_cmd;

        std::string _serialized_string;

    public:

        explicit pipeline_serializer(
            std::vector<std::string> const & parsed_cmd
        );

        std::string get_serialized_command();

        pipeline_serializer & serialize_char(
            char symbol
        );

        pipeline_serializer & serialize_unsigned_long(
            unsigned long number
        );

        pipeline_serializer & serialize_string(
            std::string const & str
        );

    };

}

#endif /* PIPELINE_SERIALIZER */

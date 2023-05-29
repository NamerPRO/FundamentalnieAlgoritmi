#ifndef PIPELINE_SERIALIZER
#define PIPELINE_SERIALIZER

#include <string>
#include <vector>

namespace npipeline_tools {

    class pipeline_serializer final {

    private:

        std::string _serialized_string;

    public:

        explicit pipeline_serializer(
            std::string & response_as_string
        );

        std::string get_serialized_response();

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

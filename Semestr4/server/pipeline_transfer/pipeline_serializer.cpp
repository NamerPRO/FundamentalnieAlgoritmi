#include "./pipeline_serializer.h"

namespace npipeline_tools {

    pipeline_serializer::pipeline_serializer(
        std::string & response_as_string
    ) {
        serialize_string(response_as_string);
    }

    std::string pipeline_serializer::get_serialized_response() {
        return _serialized_string;
    }

    pipeline_serializer & pipeline_serializer::serialize_char(
        char symbol
    ) {
        _serialized_string += symbol;
        return *this;
    }

    pipeline_serializer & pipeline_serializer::serialize_unsigned_long(
        unsigned long number
    ) {
        _serialized_string += (number < 0) ? '0' : '1';
        number = (number < 0) ? -number : number;
        for (int i = 0; i < sizeof(unsigned long); ++i) {
            _serialized_string += std::to_string(number & 255);
            number >>= 8;
        }
        return *this;
    }

    pipeline_serializer & pipeline_serializer::serialize_string(
        std::string const & str
    ) {
        unsigned int string_length = str.size();
        serialize_unsigned_long(string_length);
        _serialized_string += str;
        return *this;
    }

}
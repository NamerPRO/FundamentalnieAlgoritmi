#include "./pipeline_serializer.h"

namespace npipeline_tools {

    pipeline_serializer::pipeline_serializer(
        std::vector<std::string> const & parsed_cmd
    ) : _parsed_cmd(parsed_cmd) {
        for (int i = 0; i < _parsed_cmd.size(); ++i) {
            serialize_string(_parsed_cmd[i]);
        }
        serialize_unsigned_long(0);
    }

    std::string pipeline_serializer::get_serialized_command() {
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
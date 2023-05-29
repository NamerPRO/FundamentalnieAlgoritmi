#include "./pipeline_deserializer.h"

namespace npipeline_tools {

    char pipeline_deserializer::deserialize_char() {
        return _serialized_string[_deserializer_index++];
    }

    unsigned long pipeline_deserializer::deserialize_unsigned_long() {
        unsigned long deserialized_number = 0;
        short is_negative = _serialized_string[_deserializer_index++] - '0';
        for (int i = 0; i < sizeof(unsigned long); ++i) {
            deserialized_number = (deserialized_number << 8) | (unsigned char)_serialized_string[_deserializer_index++];
        }
        return is_negative ? deserialized_number : -deserialized_number;
    }

    std::string pipeline_deserializer::deserialize_string() {
        unsigned long string_length = deserialize_unsigned_long();
        std::string deserialized_string;
        for (int i = 0; i < string_length; ++i) {
            deserialized_string += _serialized_string[_deserializer_index++];
        }
        return deserialized_string;
    }

    std::string pipeline_deserializer::deserialize_response() {
        return deserialize_string();
    }

}
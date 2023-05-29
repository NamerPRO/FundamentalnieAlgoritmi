#ifndef PIPELINE_DESERIALIZER_H_
#define PIPELINE_DESERIALIZER_H_

#include <string>
#include <vector>

namespace npipeline_tools {

    class pipeline_deserializer final {

    private:

        std::string _serialized_string;

        unsigned int _deserializer_index = 0;

    public:

        explicit pipeline_deserializer(
            std::string & serialized_string
        ) : _serialized_string(serialized_string) {}

        char deserialize_char();

        unsigned long deserialize_unsigned_long();

        std::string deserialize_string();

        std::vector<std::string> deserialize_request();

    };

}

#endif // PIPELINE_DESERIALIZER_H_
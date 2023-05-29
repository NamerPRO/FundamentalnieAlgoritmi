#include "./pipeline_receiver.h"

namespace npipeline_tools {

    std::string receiver::receive(
        char * uuid_ptr
    ) {
        shared_memory_manager & shared_memory = shared_memory_manager::get_instance();
        std::pair<bool, std::string> response;
        while (!(response = shared_memory.ping(uuid_ptr)).first) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        pipeline_deserializer deserializer(response.second);
        return deserializer.deserialize_response();
    }

}
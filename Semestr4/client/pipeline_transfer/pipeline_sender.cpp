#include "./pipeline_sender.h"

namespace npipeline_tools {

    sender & sender::add(
        std::vector<std::string> const & parsed_cmd
    ) {
        pipeline_serializer serializer(parsed_cmd);
        _requests_list.push_front(std::move(serializer.get_serialized_command()));
        return *this;
    }

    char * sender::send(
        char * uuid_ptr
    ) {
        shared_memory_manager & shared_memory = shared_memory_manager::get_instance();
        uuid_ptr = shared_memory.write(_requests_list.back().c_str(), uuid_ptr);
        _requests_list.pop_back();
        return uuid_ptr;
    }

}
#include <Windows.h>

#include <iostream>

#include "./ipc_manager/semaphores.h"
#include "./ipc_manager/shared_memory.h"

#include "./pipeline_transfer/pipeline_deserializer.h"
#include "./pipeline_transfer/pipeline_serializer.h"

#include "./storage_manager/data_storage.h"

#include "./nmemory/buddy_memory_allocator.h"
#include "./nmemory/sorted_list_memory.h"
#include "./nmemory/memory_boundary_descriptors.h"
#include "./nmemory/allocate_type_helper.h"

#include "./pipeline_manager/pipeline.h"

// Make commands write result in data_storage

// Implement support when client pings server

int main() {
    npipeline_tools::semaphore_manager & semaphores = npipeline_tools::semaphore_manager::get_instance();
    
    npipeline_tools::storage & storage = npipeline_tools::storage::get_instanse();

    while (true) {
        
        if (WaitForSingleObject(semaphores.get(npipeline_tools::semaphore_manager::type::server), INFINITE) != 0) {
            throw std::runtime_error("An error occured while attemption to decrement semaphore!");
        }

        npipeline_tools::shared_memory_manager & shared_memory_block = npipeline_tools::shared_memory_manager::get_instance();
        char * shared_memory = shared_memory_block.get_shared_memory();

        bool is_uuid_requested = shared_memory[1] == 'g';
        char * uuid_ptr;

        unsigned int standard_uuid_size = npipeline_tools::shared_memory_manager::get_standard_uuid_size();

        // Get uuid
        if (is_uuid_requested) {
            UUID uuid;
            UuidCreate(&uuid);
            UuidToString(&uuid, reinterpret_cast<RPC_CSTR *>(&uuid_ptr));
        } else {
            uuid_ptr = new char[standard_uuid_size + 1];
            std::memcpy(uuid_ptr, shared_memory + 2, standard_uuid_size);
            uuid_ptr[standard_uuid_size] = '\0';
        }

        std::string uuid_as_string(uuid_ptr);

        unsigned int standard_block_size = npipeline_tools::shared_memory_manager::get_standard_block_size();
        
        int characters_in_request = standard_block_size - 2 - (is_uuid_requested ? standard_uuid_size : 0);
        
        char * request = new char[characters_in_request + 1];
        std::memcpy(request, shared_memory + 2 + (is_uuid_requested ? standard_uuid_size : 0), sizeof(char) * characters_in_request);
        request[characters_in_request] = '\0';

        storage.get_requests()[uuid_as_string] += request;

        // If we got whole response then we need to deserialize and run command
        if (shared_memory[0] == 'f') {
            npipeline_tools::pipeline_deserializer deserializer(storage.get_requests()[uuid_as_string]);
            std::vector<std::string> tokens_list = deserializer.deserialize_request();

            npipeline::pipeline & current_pipeline = storage.get_pipelines()[uuid_as_string];

            if (tokens_list[0] == "CREATE_PIPELINE") {
                nmemory::memory * allocator = nullptr;
                if (tokens_list[2] == "buddy") {
                    allocator = new nmemory_standards::buddy_memory_allocator(
                        nullptr,
                        std::stoul(tokens_list[3]),
                        nullptr
                    );
                } else if (tokens_list[2] == "sorted") {
                    allocator = new nmemory_standards::sorted_list_memory(
                        nullptr,
                        std::stoul(tokens_list[3]),
                        (tokens_list[4] == "first_fit") ? nmemory_standards::allocate_type_helper::allocate_type::first_fit : (tokens_list[4] == "best_fit") ? nmemory_standards::allocate_type_helper::allocate_type::best_fit : nmemory_standards::allocate_type_helper::allocate_type::worst_fit,
                        nullptr
                    );
                } else if (tokens_list[2] == "boundary") {
                    allocator = new nmemory_standards::boundary_descriptors_memory(
                        nullptr,
                        std::stoul(tokens_list[3]),
                        (tokens_list[4] == "first_fit") ? nmemory_standards::allocate_type_helper::allocate_type::first_fit : (tokens_list[4] == "best_fit") ? nmemory_standards::allocate_type_helper::allocate_type::best_fit : nmemory_standards::allocate_type_helper::allocate_type::worst_fit,
                        nullptr
                    );
                }
                storage.get_pipelines()[uuid_as_string] = npipeline::pipeline(
                    (tokens_list[1] == "avl") ? npipeline::pipeline_base::tree_type::avl : (tokens_list[1] == "splay") ? npipeline::pipeline_base::tree_type::splay : npipeline::pipeline_base::tree_type::btree,
                    allocator
                );
                storage.get_responses()[uuid_as_string] = "Pipeline was successfully created!";
            } else if (tokens_list[0] == "DESTROY_PIPELINE") {
                storage.get_pipelines().erase(uuid_as_string);
                storage.get_responses()[uuid_as_string] = "Pipeline was successfully destroyed!";
            } else if (tokens_list[0] == "SAVE") {
                npipeline_tools::data_save saver(
                    std::move(tokens_list[1]),
                    current_pipeline.get_dbase(),
                    current_pipeline.get_dbase_with_developer_login_key(),
                    current_pipeline._main_tree_type
                );
                saver.save();
                storage.get_responses()[uuid_as_string] = "Data was successfully saved!";
            } else if (tokens_list[0] == "RESTORE") {
                npipeline_tools::data_save restorer(
                    std::move(tokens_list[1]),
                    current_pipeline.get_dbase(),
                    current_pipeline.get_dbase_with_developer_login_key(),
                    current_pipeline._main_tree_type
                );
                restorer.restore();
                storage.get_responses()[uuid_as_string] = "Data was successfully restored!";
            } else {
                npipeline_tools::command * cmd = storage.get_actions()[tokens_list[0]]->create_command(
                    current_pipeline.get_dbase(),
                    current_pipeline.get_dbase_with_developer_login_key(),
                    tokens_list,
                    current_pipeline.get_allocator(),
                    uuid_as_string
                );
                current_pipeline.get_invoker()->add(cmd).invoke();
            }
            // If uuid was requested from server RETURN generated one.
            if (is_uuid_requested) {
                std::memcpy(shared_memory, uuid_ptr, sizeof(char) * standard_uuid_size);
                RpcStringFree(reinterpret_cast<RPC_CSTR *>(&uuid_ptr));
            } else {
                delete uuid_ptr;
            }

            // Unlock certain client
            if (ReleaseSemaphore(semaphores.get(npipeline_tools::semaphore_manager::type::certain_client), 1, NULL) == 0) {
                throw std::runtime_error("An error occured while attemption to release semaphore!");
            }
        } else if (shared_memory[0] == 'r') {
            std::string response = storage.get_responses()[uuid_as_string];
            if (response.empty()) {
                shared_memory[0] = 'n';
            } else {
                shared_memory[0] = 'y';
                npipeline_tools::pipeline_serializer serializer(response);
                std::string serialized_response = serializer.get_serialized_response();
                shared_memory_block.write(serialized_response);
            }
            delete uuid_ptr;
        }
        
        delete[] request;
    }

    return 0;
}
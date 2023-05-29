#ifndef PIPELINE_RECEIVER_H_
#define PIPELINE_RECEIVER_H_

#include <thread>
#include <chrono>

#include "../ipc_manager/shared_memory.h"

#include "../transfer_manager/standard_receiver.h"

#include "./pipeline_deserializer.h"

namespace npipeline_tools {

    class receiver final : public ntransfer::standard_receiver {

    public:
    
        std::string receive(
            char * uuid_ptr
        ) override;

    };

}

#endif // PIPELINE_RECEIVER_H_
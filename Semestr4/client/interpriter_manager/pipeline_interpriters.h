#ifndef PIPELINE_INTERPRITER
#define PIPELINE_INTERPRITER

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "./interpriter.h"

#include "../pipeline_transfer/pipeline_sender.h"
#include "../pipeline_transfer/pipeline_receiver.h"

namespace npipeline_tools {

    class pipeline_interpriter final : public ninterpriter::interpriter {

    private:
        
        std::string _file_to_interpritate_path;

        char * _uuid_ptr = nullptr;

    public:

        explicit pipeline_interpriter(
            char * file_to_interpritate_path
        ) : _file_to_interpritate_path(file_to_interpritate_path) {}

        void interpritate() override;

        ~pipeline_interpriter() {
            delete _uuid_ptr;
        }

    };

    class user_interpriter final : public ninterpriter::interpriter {

    public:

        void interpritate() override;

        ~user_interpriter() = default;

    };

}

#endif /* PIPELINE_INTERPRITER */

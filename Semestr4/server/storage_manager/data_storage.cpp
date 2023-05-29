#include "./data_storage.h"

namespace npipeline_tools {

    std::unordered_map<std::string, std::string> & storage::get_requests() {
        return _requests;
    }

    std::unordered_map<std::string, std::string> & storage::get_responses() {
        return _responses;
    }

    std::unordered_map<std::string, npipeline::pipeline> & storage::get_pipelines() {
        return _pipelines;
    }

    std::unordered_map<std::string, command_creator *> & storage::get_actions() {
        return _actions;
    }

    storage::~storage() {
        auto it_end = _actions.end();
        for (auto it_begin = _actions.begin(); it_begin != it_end; ++it_begin) {
            delete it_begin->second;
        }
    }

    storage & storage::get_instanse() {
        static storage singelton_storage;
        return singelton_storage;
    }

}
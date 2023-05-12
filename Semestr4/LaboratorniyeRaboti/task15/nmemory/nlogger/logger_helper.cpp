#include "logger_helper.h"

nmemory_standards::logger_help::logger_help(nlogger::logger *memory_logger) {
    _memory_logger = memory_logger;
}

void nmemory_standards::logger_help::log_memory(std::string const & message, nlogger::logger::severity level) const {
    if (this->_memory_logger == nullptr) {
        return;
    }
    _memory_logger->log(message, level);
}

bool nmemory_standards::logger_help::has_logger() const noexcept {
    return _memory_logger != nullptr;
}

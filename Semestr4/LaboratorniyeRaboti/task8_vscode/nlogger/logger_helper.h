#ifndef TASK4_LOGGER_HELP_H
#define TASK4_LOGGER_HELP_H

#include <iostream>

#include "standard_logger.h"
#include "standard_logger_builder.h"

namespace nmemory_standards {

    class logger_help {

    private:

        nlogger::logger * _memory_logger;

    protected:

        explicit logger_help(nlogger::logger * memory_logger);

        bool has_logger() const noexcept;

        void log_memory(std::string const & message, nlogger::logger::severity level) const;

    };

}

#endif //TASK4_LOGGER_HELP_H

#ifndef ___STANDARD_LOGGER___
#define ___STANDARD_LOGGER___

#include <string>
#include <map>
#include <fstream>

#include "logger.h"
#include "basics.h"
#include "cpp_time.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class standard_logger final : public nlogger::logger {

private:

	std::map<nlogger::logger::severity, std::string> _streams;

	static void get_string_severity(std::string& str, nlogger::logger::severity level);

	void close();

public:

	standard_logger(logger const &) = delete;

	standard_logger & operator= (standard_logger const &) = delete;

	// Хотечется, чтобы поле было приватным, но
	// к нему нужен доступ из standard_logger_builder...
	static std::map<std::string, std::pair<std::ostream*, int>> _descriptors;

	nlogger::logger* log(const std::string& target, nlogger::logger::severity level) override;

	standard_logger(std::map<nlogger::logger::severity, std::string> streams);

	~standard_logger();

};

#endif

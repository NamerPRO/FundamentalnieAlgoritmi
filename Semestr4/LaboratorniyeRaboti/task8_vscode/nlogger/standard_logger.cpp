#include "standard_logger.h"

std::map<std::string, std::pair<std::ostream*, int>> standard_logger::_descriptors;

standard_logger::standard_logger(std::map<nlogger::logger::severity, std::string> streams) {
	_streams = streams;
}

standard_logger::~standard_logger() {
	this->close();
}

nlogger::logger* standard_logger::log(const std::string& target, nlogger::logger::severity level) {
	char logger_time[20];
	std::string logger_severity;

	try {
		get_current_time(logger_time, sizeof(logger_time));
	} catch (std::runtime_error error) {
		strcpy(logger_time, "UndefinedTime!");
	}
	get_string_severity(logger_severity, level);

	for (int i = (int)level; i >= 0; --i) {
		if (_streams.contains(severity(i))) {
			*(_descriptors[_streams[severity(i)]].first) << logger_time << ' ' << logger_severity << ' ' << target << std::endl;
		}
	}
	return this;
}

void standard_logger::close() {
	for (auto const& elem : _streams) {
		--_descriptors[elem.second].second;
		if (_descriptors[elem.second].second == 0) {
			if (_descriptors[elem.second].first->rdbuf() != std::cout.rdbuf()) {
				static_cast<std::ofstream*>(_descriptors[elem.second].first)->close();
				delete _descriptors[elem.second].first;
			}
			_descriptors.erase(elem.second);
		}
	}
}

void standard_logger::get_string_severity(std::string& str, nlogger::logger::severity level) {
	switch (level) {
	case nlogger::logger::severity::trace:
		str = "TRACE";
		break;
	case nlogger::logger::severity::debug:
		str = "DEBUG";
		break;
	case nlogger::logger::severity::information:
		str = "INFORMATION";
		break;
	case nlogger::logger::severity::warning:
		str = "WARNING";
		break;
	case nlogger::logger::severity::error:
		str = "ERROR";
		break;
	case nlogger::logger::severity::critical:
		str = "CRITICAL";
		break;
	}
}

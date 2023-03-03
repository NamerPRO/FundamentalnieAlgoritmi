#include "standard_logger.hpp"

map<string, pair<ostream*, int>> standard_logger::_descriptors;

standard_logger::standard_logger(map<severity, string> streams) {
	_streams = streams;
}

standard_logger::~standard_logger() {
	this->close();
}

logger* standard_logger::log(const string& target, logger::severity level) {
	char logger_time[20];
	string logger_severity;

	try {
		get_current_time(logger_time, sizeof(logger_time));
	} catch (runtime_error error) {
		strcpy(logger_time, "UndefinedTime!");
	}
	get_string_severity(logger_severity, level);

	for (int i = (int)level; i >= 0; --i) {
		if (_streams.contains(severity(i))) {
			*(_descriptors[_streams[severity(i)]].first) << logger_time << ' ' << logger_severity << ' ' << target << endl;
		}
	}
	return this;
}

void standard_logger::close() {
	for (auto const& elem : _streams) {
		--_descriptors[elem.second].second;
		if (_descriptors[elem.second].second == 0) {
			if (_descriptors[elem.second].first->rdbuf() != cout.rdbuf()) {
				static_cast<ofstream*>(_descriptors[elem.second].first)->close();
				delete _descriptors[elem.second].first;
			}
			_descriptors.erase(elem.second);
		}
	}
}

void standard_logger::get_string_severity(string& str, logger::severity level) {
	switch (level) {
	case logger::severity::trace:
		str = "TRACE";
		break;
	case logger::severity::debug:
		str = "DEBUG";
		break;
	case logger::severity::information:
		str = "INFORMATION";
		break;
	case logger::severity::warning:
		str = "WARNING";
		break;
	case logger::severity::error:
		str = "ERROR";
		break;
	case logger::severity::critical:
		str = "CRITICAL";
		break;
	}
}

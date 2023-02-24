#include "logger.hpp"

map<string, pair<ostream*, int>> logger::_descriptors;

logger* logger::log(const string& target, severity level) {
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

void logger::close() {
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

void logger::get_string_severity(string& str, severity level) {
	switch (level) {
	case severity::trace:
		str = "TRACE";
		break;
	case severity::debug:
		str = "DEBUG";
		break;
	case severity::information:
		str = "INFORMATION";
		break;
	case severity::warning:
		str = "WARNING";
		break;
	case severity::error:
		str = "ERROR";
		break;
	case severity::critical:
		str = "CRITICAL";
		break;
	}
}

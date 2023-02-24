#include "logger_builder.hpp"

logger_builder& logger_builder::bind(string path, severity level) {
	if (_streams.contains(level)) {
		if (logger::_descriptors[_streams[level]].second == 0) {
			if (logger::_descriptors[_streams[level]].first->rdbuf() != cout.rdbuf()) {
				static_cast<ofstream*>(logger::_descriptors[_streams[level]].first)->close();
				delete logger::_descriptors[_streams[level]].first;
			}
			logger::_descriptors.erase(_streams[level]);
		}
	}
	_streams[level] = path;
	if (!logger::_descriptors.contains(path)) {
		try {
			if (path == ":console") {
				logger::_descriptors[path].first = &cout;
				logger::_descriptors[path].second = 0;
			} else {
				ofstream* file_stream = new ofstream(path, ios::app);
				if (!file_stream->is_open()) {
					delete file_stream;
					this->comb();
					throw runtime_error("An error occured while trying to interract with file.");
				}
				logger::_descriptors[path].first = file_stream;
				logger::_descriptors[path].second = 0;
			}
		} catch (const bad_alloc& error) {
			this->comb();
			throw runtime_error("Memory allocate exception.");
		}
	}
	return *this;
}

logger* logger_builder::build() {
	for (auto const& elem : _streams) {
		++logger::_descriptors[elem.second].second;
	}
	return new logger(_streams);
}

void logger_builder::close_all() {
	for (auto const& elem : logger::_descriptors) {
		if (elem.second.first->rdbuf() != cout.rdbuf()) {
			static_cast<ofstream*>(elem.second.first)->close();
			delete elem.second.first;
		}
	}
	logger::_descriptors.clear();
}

bool logger_builder::is_severity(string level) {
	to_lower_case(level);
	return level == "trace" || level == "debug" || level == "information" ||
		level == "warning" || level == "error" || level == "critical";
}

severity logger_builder::to_severity(string level) {
	to_lower_case(level);
	if (level == "trace") {
		return severity::trace;
	} else if (level == "debug") {
		return severity::debug;
	} else if (level == "information") {
		return severity::information;
	} else if (level == "warning") {
		return severity::warning;
	} else if (level == "error") {
		return severity::error;
	} else if (level == "critical") {
		return severity::critical;
	}
}

void logger_builder::comb() {
	for (auto const& elem : _streams) {
		if (logger::_descriptors[elem.second].second == 0) {
			if (logger::_descriptors[elem.second].first->rdbuf() != cout.rdbuf()) {
				static_cast<ofstream*>(logger::_descriptors[elem.second].first)->close();
				delete logger::_descriptors[elem.second].first;
			}
			logger::_descriptors.erase(elem.second);
		}
	}
}

logger* logger_builder::from_file(const string& file) {
	ifstream configuration(file);
	if (!configuration.is_open()) {
		throw runtime_error("Failed to initialize logger via file due to file does not exist.");
	}
	if (!json::accept(configuration)) {
		throw runtime_error("Invalid json file format! Wrong syntax.");
	}
	configuration.clear();
	configuration.seekg(0, ios::beg);
	json data = json::parse(configuration);
	logger_builder builder;
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (it->is_structured()) {
			builder.comb();
			throw runtime_error("Invalid json file format! Unexpected nesting.");
		}
		string string_severity = it.key();
		if (!is_severity(string_severity)) {
			builder.comb();
			throw runtime_error("Invalid json file format! Unknown severity.");
		}
		severity level = to_severity(string_severity);
		if (!it.value().is_string()) {
			builder.comb();
			throw runtime_error("Invalid json file format! Expected path to file.");
		}
		builder.bind(it.value(), level);
	}
	return builder.build();
}

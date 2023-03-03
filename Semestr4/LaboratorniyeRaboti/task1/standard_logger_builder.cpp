#include "standard_logger_builder.hpp"

standard_logger_builder::~standard_logger_builder() {}

logger_builder* standard_logger_builder::bind(string const & path, logger::severity level) {
	if (_streams.contains(level)) {
		if (standard_logger::_descriptors[_streams[level]].second == 0) {
			if (standard_logger::_descriptors[_streams[level]].first->rdbuf() != cout.rdbuf()) {
				static_cast<ofstream*>(standard_logger::_descriptors[_streams[level]].first)->close();
				delete standard_logger::_descriptors[_streams[level]].first;
			}
			standard_logger::_descriptors.erase(_streams[level]);
		}
	}
	_streams[level] = path;
	if (!standard_logger::_descriptors.contains(path)) {
		try {
			if (path == ":console") {
				standard_logger::_descriptors[path].first = &cout;
				standard_logger::_descriptors[path].second = 0;
			} else {
				ofstream* file_stream = new ofstream(path, ios::app);
				if (!file_stream->is_open()) {
					delete file_stream;
					this->comb();
					throw runtime_error("An error occured while trying to interract with file.");
				}
				standard_logger::_descriptors[path].first = file_stream;
				standard_logger::_descriptors[path].second = 0;
			}
		} catch (const bad_alloc& error) {
			this->comb();
			throw runtime_error("Memory allocate exception.");
		}
	}
	return this;
}

logger* standard_logger_builder::build() {
	for (auto const& elem : _streams) {
		++standard_logger::_descriptors[elem.second].second;
	}
	return new standard_logger(_streams);
}

void standard_logger_builder::close_all() {
	for (auto const& elem : standard_logger::_descriptors) {
		if (elem.second.first->rdbuf() != cout.rdbuf()) {
			static_cast<ofstream*>(elem.second.first)->close();
			delete elem.second.first;
		}
	}
	standard_logger::_descriptors.clear();
}

bool standard_logger_builder::is_severity(string level) {
	to_lower_case(level);
	return level == "trace" || level == "debug" || level == "information" ||
		level == "warning" || level == "error" || level == "critical";
}

logger::severity standard_logger_builder::to_severity(string level) {
	to_lower_case(level);
	if (level == "trace") {
		return logger::severity::trace;
	} else if (level == "debug") {
		return logger::severity::debug;
	} else if (level == "information") {
		return logger::severity::information;
	} else if (level == "warning") {
		return logger::severity::warning;
	} else if (level == "error") {
		return logger::severity::error;
	} else if (level == "critical") {
		return logger::severity::critical;
	}
}

void standard_logger_builder::comb() {
	for (auto const& elem : _streams) {
		if (standard_logger::_descriptors[elem.second].second == 0) {
			if (standard_logger::_descriptors[elem.second].first->rdbuf() != cout.rdbuf()) {
				static_cast<ofstream*>(standard_logger::_descriptors[elem.second].first)->close();
				delete standard_logger::_descriptors[elem.second].first;
			}
			standard_logger::_descriptors.erase(elem.second);
		}
	}
}

logger* standard_logger_builder::from_file(const string& file) {
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
	logger_builder* builder = new standard_logger_builder();
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (it->is_structured()) {
			reinterpret_cast<standard_logger_builder*>(builder)->comb();
			throw runtime_error("Invalid json file format! Unexpected nesting.");
		}
		string string_severity = it.key();
		if (!is_severity(string_severity)) {
			reinterpret_cast<standard_logger_builder*>(builder)->comb();
			throw runtime_error("Invalid json file format! Unknown severity.");
		}
		logger::severity level = to_severity(string_severity);
		if (!it.value().is_string()) {
			reinterpret_cast<standard_logger_builder*>(builder)->comb();
			throw runtime_error("Invalid json file format! Expected path to file.");
		}
		builder->bind(it.value(), level);
	}
	logger* json_logger = builder->build();
	delete builder;
	return json_logger;
}

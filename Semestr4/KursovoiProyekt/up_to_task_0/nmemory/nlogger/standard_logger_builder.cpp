#include "standard_logger_builder.h"

standard_logger_builder::~standard_logger_builder() = default;

nlogger::logger_builder* standard_logger_builder::bind(std::string const & path, nlogger::logger::severity level) {
	if (_streams.contains(level)) {
		if (standard_logger::_descriptors[_streams[level]].second == 0) {
			if (standard_logger::_descriptors[_streams[level]].first->rdbuf() != std::cout.rdbuf()) {
				static_cast<std::ofstream*>(standard_logger::_descriptors[_streams[level]].first)->close();
				delete standard_logger::_descriptors[_streams[level]].first;
			}
			standard_logger::_descriptors.erase(_streams[level]);
		}
	}
	_streams[level] = path;
	if (!standard_logger::_descriptors.contains(path)) {
		try {
			if (path == ":console") {
				standard_logger::_descriptors[path].first = &std::cout;
				standard_logger::_descriptors[path].second = 0;
			} else {
				std::ofstream* file_stream = new std::ofstream(path, std::ios::app);
				if (!file_stream->is_open()) {
					delete file_stream;
					this->comb();
					throw std::runtime_error("An error occured while trying to interract with file.");
				}
				standard_logger::_descriptors[path].first = file_stream;
				standard_logger::_descriptors[path].second = 0;
			}
		} catch (const std::bad_alloc& error) {
			this->comb();
			throw std::runtime_error("Memory allocate exception.");
		}
	}
	return this;
}

nlogger::logger* standard_logger_builder::build() {
	for (auto const& elem : _streams) {
		++standard_logger::_descriptors[elem.second].second;
	}
	return new standard_logger(_streams);
}

void standard_logger_builder::close_all() {
	for (auto const& elem : standard_logger::_descriptors) {
		if (elem.second.first->rdbuf() != std::cout.rdbuf()) {
			static_cast<std::ofstream*>(elem.second.first)->close();
			delete elem.second.first;
		}
	}
	standard_logger::_descriptors.clear();
}

bool standard_logger_builder::is_severity(std::string level) {
	to_lower_case(level);
	return level == "trace" || level == "debug" || level == "information" ||
		level == "warning" || level == "error" || level == "critical";
}

nlogger::logger::severity standard_logger_builder::to_severity(std::string level) {
	to_lower_case(level);
	if (level == "trace") {
		return nlogger::logger::severity::trace;
	} else if (level == "debug") {
		return nlogger::logger::severity::debug;
	} else if (level == "information") {
		return nlogger::logger::severity::information;
	} else if (level == "warning") {
		return nlogger::logger::severity::warning;
	} else if (level == "error") {
		return nlogger::logger::severity::error;
	} else if (level == "critical") {
		return nlogger::logger::severity::critical;
	}
}

void standard_logger_builder::comb() {
	for (auto const& elem : _streams) {
		if (standard_logger::_descriptors[elem.second].second == 0) {
			if (standard_logger::_descriptors[elem.second].first->rdbuf() != std::cout.rdbuf()) {
				static_cast<std::ofstream*>(standard_logger::_descriptors[elem.second].first)->close();
				delete standard_logger::_descriptors[elem.second].first;
			}
			standard_logger::_descriptors.erase(elem.second);
		}
	}
}

nlogger::logger* standard_logger_builder::from_file(const std::string& file) {
	std::ifstream configuration(file);
	if (!configuration.is_open()) {
		throw std::runtime_error("Failed to initialize logger via file due to file does not exist.");
	}
	if (!json::accept(configuration)) {
		throw std::runtime_error("Invalid json file format! Wrong syntax.");
	}
	configuration.clear();
	configuration.seekg(0, std::ios::beg);
	json data = json::parse(configuration);
	nlogger::logger_builder* builder = new standard_logger_builder();
	for (auto it = data.begin(); it != data.end(); ++it) {
		if (it->is_structured()) {
			reinterpret_cast<standard_logger_builder*>(builder)->comb();
			throw std::runtime_error("Invalid json file format! Unexpected nesting.");
		}
		std::string string_severity = it.key();
		if (!is_severity(string_severity)) {
			reinterpret_cast<standard_logger_builder*>(builder)->comb();
			throw std::runtime_error("Invalid json file format! Unknown severity.");
		}
		nlogger::logger::severity level = to_severity(string_severity);
		if (!it.value().is_string()) {
			reinterpret_cast<standard_logger_builder*>(builder)->comb();
			throw std::runtime_error("Invalid json file format! Expected path to file.");
		}
		builder->bind(it.value(), level);
	}
	nlogger::logger* json_logger = builder->build();
	delete builder;
	return json_logger;
}

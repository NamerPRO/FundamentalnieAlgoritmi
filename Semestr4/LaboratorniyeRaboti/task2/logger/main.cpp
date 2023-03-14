#include <iostream>
#include <stdexcept>

#include "standard_logger.hpp"
#include "standard_logger_builder.hpp"

#define UNKNOWN_ERROR 1

int main() {
	try {

		// ===
		nlogger::logger_builder* builder_one = new standard_logger_builder();

		nlogger::logger* logger_one = builder_one->bind("file1.txt", nlogger::logger::severity::debug)
																	->bind("file2.txt", nlogger::logger::severity::warning)
																	->bind(":console", nlogger::logger::severity::trace)
																	->build();

		logger_one->log("This is an example log message.", nlogger::logger::severity::warning)
						->log("Debug helps you very much...", nlogger::logger::severity::critical)
						->log("Logging very important information!", nlogger::logger::severity::error);

		delete logger_one;
		delete builder_one;
		// ===

		// ===
		nlogger::logger* logger_two = standard_logger_builder().from_file("./json_logger_configurations/logger1.json");

		logger_two->log("App crashed! Cannot procceed :(", nlogger::logger::severity::critical)
						->log("This is an information you need to know.", nlogger::logger::severity::debug)
						->log("This logger is made via file.", nlogger::logger::severity::information);

		delete logger_two;
		// ===

		// ===
		nlogger::logger* logger_three = standard_logger_builder().from_file("./json_logger_configurations/logger2.json");

		logger_three->log("Another logger made via file configuration.", nlogger::logger::severity::trace);

		delete logger_three;
		// ===

	} catch (std::runtime_error error) {
		std::cerr << "Unhandleable error: " << error.what() << std::endl;
		return UNKNOWN_ERROR;
	}
	return 0;
}

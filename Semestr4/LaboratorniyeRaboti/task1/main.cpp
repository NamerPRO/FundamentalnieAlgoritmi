#include <iostream>
#include <stdexcept>

#include "standard_logger.hpp"
#include "standard_logger_builder.hpp"

#define UNKNOWN_ERROR 1

using namespace std;

int main() {
	try {

		// ===
		logger_builder* builder_one = new standard_logger_builder();

		logger* logger_one = builder_one->bind("file1.txt", logger::severity::debug)
																	->bind("file2.txt", logger::severity::warning)
																	->bind(":console", logger::severity::trace)
																	->build();

		logger_one->log("This is an example log message.", logger::severity::warning)
						->log("Debug helps you very much...", logger::severity::critical)
						->log("Logging very important information!", logger::severity::error);

		delete logger_one;
		delete builder_one;
		// ===

		// ===
		logger* logger_two = standard_logger_builder().from_file("./json_logger_configurations/logger1.json");

		logger_two->log("App crashed! Cannot procceed :(", logger::severity::critical)
						->log("This is an information you need to know.", logger::severity::debug)
						->log("This logger is made via file.", logger::severity::information);

		delete logger_two;
		// ===

		// ===
		logger* logger_three = standard_logger_builder().from_file("./json_logger_configurations/logger2.json");

		logger_three->log("Another logger made via file configuration.", logger::severity::trace);

		delete logger_three;
		// ===

	} catch (runtime_error error) {
		cerr << "Unhandleable error: " << error.what() << endl;
		return UNKNOWN_ERROR;
	}
	return 0;
}

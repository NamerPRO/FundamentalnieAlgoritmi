#include <iostream>
#include <stdexcept>

#include "logger.hpp"
#include "logger_builder.hpp"

#define UNKNOWN_ERROR 1

using namespace std;

int main() {
	try {
		// ===
		logger_builder builder_one = logger_builder().bind("file1.txt", severity::debug).bind("file2.txt", severity::warning)
																									.bind(":console", severity::trace);
		logger* logger_one = builder_one.build();
		logger_one->log("This is an example log message.", severity::warning)
								->log("Debug helps you very much...", severity::critical)
								->log("Logging very important information!", severity::error);
		delete logger_one;
		// ===

		// ===
		logger* logger_two = logger_builder::from_file("./json_logger_configurations/logger1.json");
		logger_two->log("App crashed! Cannot procceed :(", severity::critical)
								->log("This is an information you need to know.", severity::debug)
								->log("This logger is made via file.", severity::information);
		delete logger_two;
		// ===

		// ===
		logger* logger_three = logger_builder::from_file("./json_logger_configurations/logger2.json");
		logger_three->log("Another logger made via file configuration.", severity::trace);
		delete logger_three;
		// ===
	} catch (runtime_error error) {
		cerr << "Unhandleable error: " << error.what() << endl;
		return UNKNOWN_ERROR;
	}
	return 0;
}

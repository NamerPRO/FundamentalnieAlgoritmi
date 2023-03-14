#include "cpp_time.hpp"

void get_current_time(char* str_time, int str_time_size) {
	if (str_time_size < 20 * sizeof(char)) {
		throw std::runtime_error("Too small string passed! Needed at least 20 symbols.");
	}
	time_t raw_time = time(NULL);
	struct tm* parsed_time;
	parsed_time = localtime(&raw_time);
	if (!strftime(str_time, str_time_size, "%d/%m/%Y %H:%M:%S", parsed_time)) {
		throw std::runtime_error("An error occured while trying to parse time.");
	}
}

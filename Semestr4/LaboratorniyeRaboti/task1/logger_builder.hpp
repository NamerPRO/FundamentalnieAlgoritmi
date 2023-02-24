#ifndef ___LOGGER_BUILDER___
#define ___LOGGER_BUILDER___

#include <map>
#include <string>

#include "logger.hpp"

using namespace std;

class logger_builder {
private:
	map<severity, string> _streams;

	void comb();
	static bool is_severity(string level);
	static severity to_severity(string level);

public:
	static void close_all();
	static logger* from_file(const string& file);
	logger_builder& bind(string path, severity level);
	logger* build();

	~logger_builder() {};

};

#endif

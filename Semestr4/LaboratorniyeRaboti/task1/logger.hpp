#ifndef ___LOGGER___
#define ___LOGGER___


#include <string>
#include <map>
#include <fstream>

#include "basics.hpp"
#include "cpp_time.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

enum class severity {
	trace,
	debug,
	information,
	warning,
	error,
	critical
};

class logger {
private:
	map<severity, string> _streams;

	static void get_string_severity(string& str, severity level);
	void close();

public:
	static map<string, pair<ostream*, int>> _descriptors;
	logger* log(const string& target, severity level);

	logger(map<severity, string> streams) {
		_streams = streams;
	}

	~logger() {
		this->close();
	}
};

#endif

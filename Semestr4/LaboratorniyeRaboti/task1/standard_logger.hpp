#ifndef ___STANDARD_LOGGER___
#define ___STANDARD_LOGGER___

#include <string>
#include <map>
#include <fstream>

#include "logger.hpp"
#include "basics.hpp"
#include "cpp_time.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class standard_logger final : public logger {

private:

	map<severity, string> _streams;

	static void get_string_severity(string& str, logger::severity level);

	void close();

public:

	// Хотечется, чтобы поле было приватным, но
	// к нему нужен доступ из standard_logger_builder...
	static map<string, pair<ostream*, int>> _descriptors;

	logger* log(const string& target, logger::severity level) override;

	standard_logger(map<logger::severity, string> streams);

	~standard_logger();

};

#endif

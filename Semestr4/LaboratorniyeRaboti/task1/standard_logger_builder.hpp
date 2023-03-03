#ifndef ___STANDARD_LOGGER_BUILDER___
#define ___STANDARD_LOGGER_BUILDER___

#include <map>
#include <string>

#include "logger_builder.hpp"
#include "standard_logger.hpp"

using namespace std;

class standard_logger_builder final : public logger_builder {

private:
	map<logger::severity, string> _streams;

	void comb();
	static bool is_severity(string level);
	static logger::severity to_severity(string level);

public:
	void close_all();
	logger* from_file(const string& file) override;
	logger_builder* bind(string const & path, logger::severity level) override;
	logger* build() override;

	~standard_logger_builder();

};

#endif

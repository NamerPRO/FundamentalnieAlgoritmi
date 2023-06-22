#ifndef ___STANDARD_LOGGER_BUILDER___
#define ___STANDARD_LOGGER_BUILDER___

#include <map>
#include <string>

#include "logger_builder.h"
#include "standard_logger.h"

class standard_logger_builder final : public nlogger::logger_builder {

private:
	std::map<nlogger::logger::severity, std::string> _streams;

	void comb();
	static bool is_severity(std::string level);
	static nlogger::logger::severity to_severity(std::string level);

public:
	void close_all();
	nlogger::logger* from_file(const std::string& file) override;
	nlogger::logger_builder* bind(std::string const & path, nlogger::logger::severity level) override;
	nlogger::logger* build() override;

	~standard_logger_builder();

};

#endif

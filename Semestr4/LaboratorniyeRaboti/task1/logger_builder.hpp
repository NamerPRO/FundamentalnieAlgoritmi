#ifndef ___LOGGER_BUILDER___
#define ___LOGGER_BUILDER___

#include <iostream>

#include "logger.hpp"

class logger_builder {

public:

  virtual logger* from_file(const string & file) = 0;

	virtual logger_builder* bind(string const & path, logger::severity level) = 0;

  virtual logger* build() = 0;

  virtual ~logger_builder();

};

#endif

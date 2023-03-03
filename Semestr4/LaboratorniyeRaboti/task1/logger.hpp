#ifndef ___LOGGER___
#define ___LOGGER___

#include <iostream>
#include <string>

using namespace std;

class logger {

public:

  enum class severity {
  	trace,
  	debug,
  	information,
  	warning,
  	error,
  	critical
  };

	virtual logger* log(const string & target, severity level) = 0;

  virtual ~logger();

};

#endif

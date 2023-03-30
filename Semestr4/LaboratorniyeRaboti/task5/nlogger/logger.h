#ifndef ___LOGGER___
#define ___LOGGER___

#include <iostream>
#include <string>

namespace nlogger {

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

  	virtual logger* log(const std::string & target, severity level) = 0;

    virtual ~logger();

  };

}

#endif

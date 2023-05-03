#include "basics.h"

void to_lower_case(std::string& str) {
	for (char& symbol : str) {
		symbol = tolower(symbol);
	}
}

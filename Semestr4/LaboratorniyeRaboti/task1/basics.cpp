#include "basics.hpp"

void to_lower_case(string& str) {
	for (char& symbol : str) {
		symbol = tolower(symbol);
	}
}

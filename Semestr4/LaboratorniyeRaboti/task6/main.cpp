#include <iostream>

#include "bigint_base_arithmetic.h"

using bigint = ninteger::bigint_base_arithmetic;

int main() {
    bigint a("1200000312132200000000000000000000000000000000000000004");
    bigint b("481948949128921");

    a += b;
    std::cout << -a << std::endl;

    return 0;
}

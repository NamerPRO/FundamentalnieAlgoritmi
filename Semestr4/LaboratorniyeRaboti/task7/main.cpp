#include <iostream>

#include "nbigint/karacuba.h"
#include "nbigint/standard_bigint.h"
#include "nlogger/logger.h"
#include "nlogger/logger_builder.h"
#include "nlogger/standard_logger.h"
#include "nlogger/standard_logger_builder.h"

using bigint = narithmetic::standard_bigint;

int main() {

    // Native multiplication

    bigint result_native;
    bigint pre_calculated_factorial_native(1);
    for (int i = 1; i <= 10000; ++i) {
        bigint number(i);
        pre_calculated_factorial_native *= number;
        result_native += pre_calculated_factorial_native;
    }

    nlogger::logger_builder * my_builder_native = new standard_logger_builder();
    nlogger::logger * my_logger_native = my_builder_native->bind("output", nlogger::logger::severity::information)->build();

    my_logger_native->log(result_native.to_string(), nlogger::logger::severity::information);

    delete my_logger_native;
    delete my_builder_native;

    // ===

    // Karacuba multiplication

    bigint result_karacuba;
    bigint pre_calculated_factorial_karacuba(1);
    narithmetic::karacuba mult;
    for (int i = 1; i <= 10000; ++i) {
        bigint number(i);
        pre_calculated_factorial_karacuba = mult.multiplication(pre_calculated_factorial_karacuba, number);
        result_karacuba += pre_calculated_factorial_karacuba;
    }

    nlogger::logger_builder * my_builder_karacuba = new standard_logger_builder();
    nlogger::logger * my_logger_karacuba = my_builder_karacuba->bind("output", nlogger::logger::severity::information)->build();
    
    my_logger_karacuba->log(result_karacuba.to_string(), nlogger::logger::severity::information);

    delete my_logger_karacuba;
    delete my_builder_karacuba;

    // ===

    return 0;
}

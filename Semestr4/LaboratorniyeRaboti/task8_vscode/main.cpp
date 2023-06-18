#include <chrono>
#include <iostream>

#include "./nbigint/fft.h"
#include "nbigint/standard_bigint.h"
#include "nlogger/logger.h"
#include "nlogger/logger_builder.h"
#include "nlogger/standard_logger.h"
#include "nlogger/standard_logger_builder.h"

int main() {
    narithmetic::fmult helper;

    narithmetic::standard_bigint mult_odd{1};
    narithmetic::standard_bigint mult_even{1};

    narithmetic::standard_bigint result{0};
    
    auto start_time = std::chrono::system_clock::now();

    for (int i = 1; i <= 10000; ++i) {
        narithmetic::standard_bigint number{i};
        if (i % 2 == 1) {
            mult_odd = helper.multiplication(mult_odd, number);
            result += mult_odd;
        } else {
            mult_even = helper.multiplication(mult_even, number);
            result += mult_even;
        }
    }

    auto end_time = std::chrono::system_clock::now();

    nlogger::logger_builder * my_builder = (new standard_logger_builder())->bind("output", nlogger::logger::severity::information);
    nlogger::logger * my_logger = my_builder->build();
    
    my_logger->log(result.to_string(), nlogger::logger::severity::information)
                ->log(std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count()), nlogger::logger::severity::information);

    delete my_logger;
    delete my_builder;

    return 0;
}
#include <iostream>
#include <random>
#include <vector>

#include "nmemory/nlogger/logger.h"
#include "nmemory/nlogger/logger_builder.h"
#include "nmemory/nlogger/standard_logger_builder.h"
#include "nredblack/red_black_tree.h"

int main() {
    std::vector<int> my_vec(10000);
    for (int i = 0; i < 10000; ++i) {
        my_vec[i] = i + 1;
    }
    std::shuffle(my_vec.begin(), my_vec.end(), std::mt19937(std::random_device()()));

    nredblack::red_black_tree<int, int> my_tree;

    for (int i = 0; i < 10000; ++i) {
        my_tree.insert(my_vec[i], std::forward<int>(my_vec[i]));
    }

    auto it_begin = my_tree.infix_iterator_begin();
    auto it_end = my_tree.infix_iterator_end();

    nlogger::logger_builder * my_builder = new standard_logger_builder();
    nlogger::logger * my_logger = my_builder->bind("output", nlogger::logger::severity::information)->build();

    while (it_begin != it_end) {
        std::string node_position;
        for (int i = 0; i < std::get<0>(*it_begin); ++i) {
            node_position += ' ';
        }
        node_position += std::to_string(std::get<2>(*it_begin));
        my_logger->log(node_position, nlogger::logger::severity::information);
        ++it_begin;
    }

    delete my_logger;
    delete my_builder;

    for (int i = 0; i < 10000; ++i) {
        my_tree.remove(i + 1);
    }

    return 0;
}

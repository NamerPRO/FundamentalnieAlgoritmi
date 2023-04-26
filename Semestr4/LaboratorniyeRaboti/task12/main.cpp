#include <iostream>
#include <random>
#include <vector>

#include "nmemory/nlogger/standard_logger.h"
#include "nmemory/nlogger/standard_logger_builder.h"
#include "navl/avl_tree.h"

int main() {
    std::vector<int> my_vec(10000);
    for (int i = 0; i < 10000; ++i) {
        my_vec[i] = i + 1;
    }
    std::shuffle(my_vec.begin(), my_vec.end(), std::mt19937(std::random_device()()));

    navl::avl_tree<int, int> my_tree;
    for (int i = 0; i < 10000; ++i) {
        my_tree.insert(my_vec[i], std::forward<int>(my_vec[i]));
    }

    nlogger::logger_builder * my_builder = (new standard_logger_builder)->bind("output.txt", nlogger::logger::severity::information);
    nlogger::logger * my_logger = my_builder->build();

    auto it_begin = my_tree.infix_iterator_begin();
    auto it_end = my_tree.infix_iterator_end();

    while (it_begin != it_end) {
        unsigned int node_height = std::get<0>(*it_begin);
        std::string node_position;
        for (int i = 0; i < node_height; ++i) {
            node_position += ' ';
        }
        my_logger->log(node_position + std::to_string(std::get<2>(*it_begin)), nlogger::logger::severity::information);
        ++it_begin;
    }

    delete my_logger;
    delete my_builder;

    for (int i = 0; i < 10000; ++i) {
        my_tree.remove(i + 1);
    }

    return 0;
}

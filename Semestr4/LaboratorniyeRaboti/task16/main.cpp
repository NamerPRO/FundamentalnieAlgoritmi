#include <iostream>
#include <random>

#include "./nbtree_plus/bplustree.h"
#include "./nmemory/nlogger/standard_logger.h"
#include "./nmemory/nlogger/standard_logger_builder.h"
#include "nmemory/nlogger/logger.h"
#include "nmemory/nlogger/logger_builder.h"

#define NUM 10000

// Сделать связывание элементов в список (добавить строчку в merge и split)

int main() {

    std::vector<int> my_vec(NUM);
    for (int i = 0; i < NUM; ++i) {
        my_vec[i] = i + 1;
    }
    std::shuffle(my_vec.begin(), my_vec.end(), std::mt19937(std::random_device()()));

    nbplustree::bplustree<int, int> my_tree(2);

    for (int i = 0; i < NUM; ++i) {
        my_tree.insert(my_vec[i], std::forward<int>(my_vec[i]));
    }

    nlogger::logger_builder * my_builder = new standard_logger_builder();
    nlogger::logger * my_logger = my_builder->bind("output", nlogger::logger::severity::information)->build();

    my_tree.print(true, my_logger);

    delete my_logger;
    delete my_builder;

    for (int i = 0; i < NUM; ++i) {
        my_tree.remove(i + 1);
    }

    // nbplustree::bplustree<int, int> my_tree{2};

    // my_tree.insert(1, 1);
    // my_tree.insert(7, 7);
    // my_tree.insert(8,8);
    // my_tree.insert(5, 5);
    // my_tree.insert(4, 4);
    // my_tree.insert(9, 9);
    // my_tree.insert(2, 2);

    // my_tree.remove(7);
    // my_tree.remove(9);
    // my_tree.remove(4);
    
    // my_tree.insert(3, 3);
    // my_tree.insert(4, 4);

    // my_tree.remove(2);
    // my_tree.remove(1);
    // my_tree.remove(8);
    // my_tree.remove(3);
    // my_tree.remove(4);
    // my_tree.remove(5);


    // my_tree.print(false);

    // auto it_begin = my_tree.uinfix_iterator_begin();
    // auto it_end = my_tree.uinfix_iterator_end();

    // while (it_begin != it_end) {
    //     std::cout << (*it_begin).first << ' ';
    //     ++it_begin;
    // }
    // std::cout << std::endl;

    // my_tree.print(false);

    return 0;
}
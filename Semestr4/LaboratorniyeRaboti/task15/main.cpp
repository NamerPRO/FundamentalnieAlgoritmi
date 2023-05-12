#include <iostream>

#include "./nbtree/btree.h"

int main() {
    nbtree::btree<int, int> my_tree(2);

    my_tree += { 582, 582 };
    my_tree += { 243, 243 };
    my_tree += { 650, 650 };
    my_tree += { 6, 6 };
    my_tree += { 54, 54 };
    my_tree += { 519, 519 };
    my_tree += { 596, 596 };
    my_tree += { 757, 757 };
    my_tree += { 978, 978 };
    my_tree += { 800, 800 };

    my_tree.insert(321, 321);
    my_tree.insert(342, 342);
    my_tree.insert(765, 765);
    my_tree.insert(534, 534);
    my_tree.insert(865, 865);
    my_tree.insert(324, 324);
    my_tree.insert(653, 653);
    my_tree.insert(698, 698);
    my_tree.insert(312, 312);
    my_tree.insert(431, 431);
    my_tree.insert(253, 253);

    // my_tree.print();

    // auto it_begin_prefix = my_tree.prefix_iterator_begin();
    // auto it_end_prefix = my_tree.prefix_iterator_end();
    
    // while (it_begin_prefix != it_end_prefix) {
    //     std::cout << std::get<2>(*it_begin_prefix) << ' ';
    //     ++it_begin_prefix;
    // }
    // std::cout << std::endl;

    auto it_begin_infix = my_tree.infix_iterator_begin();
    auto it_end_infix = my_tree.infix_iterator_end();

    while (it_begin_infix != it_end_infix) {
        std::cout << std::get<1>(*it_begin_infix) << ' ';
        ++it_begin_infix;
    }
    std::cout << std::endl;

    // auto it_begin_postfix = my_tree.postfix_iterator_begin();
    // auto it_end_postfix = my_tree.postfix_iterator_end();

    // while (it_begin_postfix != it_end_postfix) {
    //     std::cout << std::get<1>(*it_begin_postfix) << ' ';
    //     ++it_begin_postfix;
    // }
    // std::cout << std::endl;

    // my_tree.print();

    // my_tree.insert(5, 5);
    // my_tree.insert(10, 10);
    // my_tree.insert(2, 2);
    // my_tree.insert(1, 1);
    // my_tree.insert(7, 7);
    // my_tree.insert(8, 8);

    // my_tree.insert(1, 1);
    // my_tree.insert(2, 2);
    // my_tree.insert(3, 3);
    // my_tree.insert(4, 4);
    // my_tree.insert(9, 9);
    // my_tree.insert(10, 10);
    // my_tree.insert(11, 11);
    // my_tree.insert(7, 7);
    // my_tree.insert(15, 15);

    // my_tree.remove(7);
    // my_tree.insert(11, 11);
    // my_tree.remove(5);
    // my_tree.remove(1); // почему values 10, а keys 2 в merge_node parent_node?
    // my_tree.remove(11);
    // my_tree.remove(2);
    // my_tree.remove(8);
    // my_tree.remove(10);

    return 0;
}
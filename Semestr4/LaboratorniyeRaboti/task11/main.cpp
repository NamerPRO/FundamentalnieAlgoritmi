#include <iostream>

#include <vector>
#include <tuple>

#include "binary_search_tree.h"
#include "./nmemory/memory.h"
#include "./nmemory/allocate_type_helper.h"
#include "./nmemory/sorted_list_memory.h"
#include "./nmemory/memory_boundary_descriptors.h"
#include "./nmemory/buddy_memory_allocator.h"

int main() {

    nmemory::memory * my_memory = new nmemory_standards::sorted_list_memory(
            nullptr,
            9999999,
            nmemory_standards::allocate_type_helper::allocate_type::worst_fit,
            nullptr
    );

    nbst::binary_search_tree<int, std::string> tree(my_memory);

    tree.insert(3, "3");
    tree.insert(2, "2");
    tree.insert(1, "1");
    tree.insert(4, std::to_string(4));

//    tree.insert(8, 8);
//    tree.insert(3, 3);
//    tree.insert(10, 10);
//    tree.insert(2, 2);
//    tree.insert(5, 5);
//    tree.insert(9, 9);
//    tree.insert(11, 11);
//    tree.insert(4, 4);
//    tree.insert(6, 6);
//    tree.insert(7, 7);
//    tree.insert(12, 12);

//    tree += { 11, 11 };
//    tree += { 9, 9 };
//    tree += { 12, 12 };
//    tree += { 3, 3 };
//    tree += { 2, 2 };
//    tree += { 5, 5 };



//    auto it_begin_prefix = tree.prefix_iterator_begin();
//    auto it_end_prefix = tree.prefix_iterator_end();
//    while (it_begin_prefix != it_end_prefix) {
//        std::cout << std::get<2>(*it_begin_prefix) << ' ';
//        ++it_begin_prefix;
//    }
//    std::cout << std::endl;

//    auto it_begin_infix = tree.infix_iterator_begin();
//    auto it_end_infix = tree.infix_iterator_end();
//    while (it_begin_infix != it_end_infix) {
//        std::cout << std::get<2>(*it_begin_infix) << ' ';
//        ++it_begin_infix;
//    }
//    std::cout << std::endl;

    auto it_begin_postfix = tree.postfix_iterator_begin();
    auto it_end_postfix = tree.postfix_iterator_end();
    while (it_begin_postfix != it_end_postfix) {
        std::cout << std::get<2>(*it_begin_postfix) << ' ';
        ++it_begin_postfix;
    }
    std::cout << std::endl;

    return 0;
}

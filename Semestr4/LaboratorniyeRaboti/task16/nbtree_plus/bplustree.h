#ifndef BPLUSTREE
#define BPLUSTREE

#include <cstdarg>
#include <iterator>
#include <stack>
#include <stdexcept>
#include <tuple>

#include "associative_container.h"
#include "../nmemory/nlogger/logger.h"
#include "nmalloc.h"

namespace nbplustree {

    class standard_comporators final {

    public:

        template<
                typename T,
                typename = std::enable_if<std::is_arithmetic<T>::value, T>::type
                >
        static bool basic_comporator(
                T const & lhs,
                T const & rhs
                ) {
            return lhs < rhs;
        }

    };

    template<typename tkey, typename tvalue, bool (*tkey_comporator)(const tkey &, const tkey &) = standard_comporators::basic_comporator>
    class bplustree : private ntools::nmalloc, public ncontainer::associative_container<tkey, tvalue, tkey_comporator> {
    
    public:
        
        explicit bplustree(
            nmemory::memory * allocator,
            unsigned int minimal_tree_power
        ) noexcept : ntools::nmalloc(allocator) {
            _root_node = nullptr;
            _minimal_tree_power = minimal_tree_power;
        }

        explicit bplustree(
            unsigned int minimal_tree_power
        ) noexcept : bplustree(nullptr, minimal_tree_power) {}

        bplustree(
            bplustree const & tree_to_copy
        ) : ntools::nmalloc(tree_to_copy._allocator) {
            clone_btree(const_cast<bplustree &>(tree_to_copy));
        }

        bplustree & operator=(
            bplustree const & tree_to_copy
        ) {
            if (&tree_to_copy == this) {
                return *this;
            }
            clear_btree();
            clone_btree(const_cast<bplustree &>(tree_to_copy));
            return *this;
        }

        bplustree(
            bplustree && tree_to_move
        ) : ntools::nmalloc(tree_to_move._allocator) {
            _minimal_tree_power = tree_to_move._minimal_tree_power;
            _root_node = tree_to_move._root_node;

            tree_to_move._root_node = nullptr;
            tree_to_move._allocator = nullptr;
        }

        bplustree & operator=(
            bplustree && tree_to_move
        ) {
            if (&tree_to_move == this) {
                return *this;
            }
            clear_btree();
            _minimal_tree_power = tree_to_move._minimal_tree_power;
            _root_node = tree_to_move._root_node;
            _allocator = tree_to_move._allocator;

            tree_to_move._root_node = nullptr;
            tree_to_move._allocator = nullptr;

            return *this;
        }

        ~bplustree() {
            clear_btree();
        }

    private:

        void clear_btree() {
            while (!empty()) {
                remove(_root_node->keys[0]);
            }
        }

        void clone_btree(
            bplustree & tree
        ) {
            _root_node = nullptr;
            _minimal_tree_power = tree._minimal_tree_power;
            _allocator = tree._allocator;
            prefix_iterator it_begin = tree.prefix_iterator_begin();
            prefix_iterator it_end = tree.prefix_iterator_end();
            while (it_begin != it_end) {
                std::tuple<unsigned int, tkey, tvalue &> node_data = *it_begin;
                insert(std::get<1>(node_data), std::move(std::get<2>(node_data)));
                ++it_begin;
            }
        }

    protected:

        struct bplustree_node : private ntools::nmalloc {
            
            bool is_leaf;
            unsigned int keys_count;
            tvalue * values;
            tkey * keys;
            bplustree_node ** sons;
            bplustree_node * next;
            // bplustree_node * past;

            bplustree_node(
                nmemory::memory * allocator,
                unsigned int minimal_tree_power,
                bool leaf_flag
            ) : ntools::nmalloc(allocator) {
                keys = ntools::nmalloc::malloc<tkey *>(sizeof(tkey) * 2 * minimal_tree_power - 1);
                values = ntools::nmalloc::malloc<tvalue *>(sizeof(tvalue) * 2 * minimal_tree_power - 1);
                sons = ntools::nmalloc::malloc<bplustree_node **>(sizeof(bplustree_node *) * 2 * minimal_tree_power);
                keys_count = 0;
                is_leaf = leaf_flag;

                // Initializing every element of keys and values arrays
                for (int i = 0; i < 2 * minimal_tree_power - 1; ++i) {
                    new (keys + i)tkey();
                    new (values + i)tvalue();
                }
            }

            ~bplustree_node() {
                ntools::nmalloc::free(keys);
                ntools::nmalloc::free(values);
                ntools::nmalloc::free(sons);
            }

        };

    private:

        unsigned int _minimal_tree_power;

        bplustree_node * _root_node;

    protected:

        // Find implementation

        tvalue & find(
            tkey const & key
        ) override {
            std::stack<bplustree_node *> nodes_stack;
            auto find_inner_data = find_inner(key, nodes_stack);
            if (find_inner_data.first == -1) {
                throw std::runtime_error("No such element exception! Cannot find element with given key in tree.");
            }
            return nodes_stack.top()->values[find_inner_data.second];
        }

    private:

        bplustree_node * find_leaf(
            tkey const & key,
            std::stack<bplustree_node *> & nodes_stack,
            // bplustree_node *& inner_node
            // std::pair<bplustree_node *, int> * inner_node_data = nullptr,
            bool * key_exists_in_inner = nullptr,
            bool first_time = true
        ) noexcept {
            bplustree_node * current_node = _root_node;

            if (key_exists_in_inner != nullptr) {
                *key_exists_in_inner = false;
            }

            if (current_node == nullptr) {        
                return nullptr;
            }

            while (true) {
                nodes_stack.push(current_node);
                if (current_node->is_leaf) {
                    break;
                }
                int left_bound = 0;
                int right_bound = current_node->keys_count - 1;
                while (left_bound <= right_bound) {
                    int middle = (left_bound + right_bound) / 2;
                    if (tkey_comporator(key, current_node->keys[middle])) { // key < current_node->keys[middle]
                        right_bound = middle - 1;
                    } else if (tkey_comporator(current_node->keys[middle], key)) { // key > current_node->keys[middle]
                        left_bound = middle + 1;
                    } else { // Element found. In this case flag it 
                        if (!first_time) {
                            return current_node;
                        }
                        
                        if (key_exists_in_inner != nullptr) {
                            *key_exists_in_inner = true;
                        }
                        
                        left_bound = middle + 1;
                        break;
                    }
                }
                current_node = current_node->sons[left_bound];
            }
            
            // while (!current_node->is_leaf) {
            //     nodes_stack.push(current_node);
            //     for (int i = 0; i <= current_node->keys_count; ++i) {
            //         if (i == current_node->keys_count || tkey_comporator(key, current_node->keys[i])) {
            //             current_node = current_node->sons[i];
            //             break;
            //         }
            //     }
            // }
            // nodes_stack.push(current_node);
            return current_node;
        }

        // Return format: { x, y }
        // Where:
        // x - index of searched element (-1 if element does not exist)
        // y - index of where element should be located (equal to x if element exists)
        std::pair<int, int> find_inner(
            tkey const & key,
            std::stack<bplustree_node *> & nodes_stack,
            // std::pair<bplustree_node *, int> * inner_node_data = nullptr,
            bool * key_exists_in_inner = nullptr,
            bool first_time = true
        ) noexcept {
            // Find leaf with data first
            find_leaf(key, nodes_stack, key_exists_in_inner, first_time);

            if (nodes_stack.empty()) {
                return { -1, 0 };
            }

            bplustree_node * concrete_node = nodes_stack.top();
            int left_bound = 0;
            int right_bound = concrete_node->keys_count - 1;
            while (left_bound <= right_bound) {
                int middle = (left_bound + right_bound) / 2;
                if (tkey_comporator(key, concrete_node->keys[middle])) { // key < current_node->keys[middle]
                    right_bound = middle - 1;
                } else if (tkey_comporator(concrete_node->keys[middle], key)) { // key > current_node->keys[middle]
                    left_bound = middle + 1;
                } else { // Element found!
                    return { middle, middle };
                    // return current_node->values[middle];
                }
            }
            return { -1, left_bound };
        }

        // ===

    public:

        // Is empty implementation

        bool empty() {
            return _root_node == nullptr;
        }

        // ===

        // Insert implementation

        void insert_inner(
            tkey const & key,
            tvalue && value,
            std::stack<bplustree_node *> nodes_stack
        ) {
            auto find_inner_data = find_inner(key, nodes_stack);
            
            if (find_inner_data.first != -1) {
                // Element with this key already exists. Perhaps update its value or do nothing
                return;
            }

            bplustree_node * node_to_move = nodes_stack.top();
            node_to_move->sons[node_to_move->keys_count + 1] = nullptr;
            for (int i = node_to_move->keys_count; i > find_inner_data.second; --i) {
                node_to_move->keys[i] = node_to_move->keys[i - 1];
                node_to_move->values[i] = std::move(node_to_move->values[i - 1]);
            }
            node_to_move->keys[find_inner_data.second] = key;
            // new (node_to_move->keys + find_inner_data.second) tkey(key);
            node_to_move->values[find_inner_data.second] = std::move(value);
            ++node_to_move->keys_count;

            // Split full nodes if needed

            bplustree_node * child_node = nodes_stack.top();
            bool is_leaf = true;
            nodes_stack.pop();
            while (!nodes_stack.empty()) {
                bplustree_node * parent_node = nodes_stack.top();
                if (child_node->keys_count == 2 * _minimal_tree_power - 1) {
                    split_node(parent_node, child_node, is_leaf);
                    is_leaf = false;
                    child_node = parent_node;
                    nodes_stack.pop();
                    continue;
                }
                break;
            }

            // Check whether we need to split root
            if (nodes_stack.empty() && _root_node->keys_count == 2 * _minimal_tree_power - 1) {
                bplustree_node * new_root_node = ntools::nmalloc::malloc<bplustree_node *>(sizeof(bplustree_node));
                new (new_root_node)bplustree_node(_allocator, _minimal_tree_power, false);

                split_node(new_root_node, _root_node, is_leaf);

                _root_node = new_root_node;
            }
        }

        void split_node(
            bplustree_node * parent_node,
            bplustree_node * node_to_split,
            bool is_leaf
        ) {
            bplustree_node * new_right_node = ntools::nmalloc::malloc<bplustree_node *>(sizeof(bplustree_node));
            new (new_right_node)bplustree_node(_allocator, _minimal_tree_power, node_to_split->is_leaf);

            int middle_element_index = _minimal_tree_power - 1;

            int synch_flag = is_leaf ? 1 : 0;
            for (int i = 0; i < middle_element_index + synch_flag; ++i) {
                new_right_node->keys[i] = node_to_split->keys[i + _minimal_tree_power - synch_flag];
                new_right_node->values[i] = std::move(node_to_split->values[i + _minimal_tree_power - synch_flag]);
                new_right_node->sons[i] = node_to_split->sons[i + _minimal_tree_power - synch_flag];
            }
            new_right_node->sons[middle_element_index + synch_flag] = node_to_split->sons[2 * _minimal_tree_power - 1];
            new_right_node->keys_count = _minimal_tree_power - 1 + synch_flag;

            // Get pushed middle elements position in parent
            int left_bound = 0;
            int right_bound = parent_node->keys_count - 1;
            while (left_bound <= right_bound) {
                int middle = (left_bound + right_bound) / 2;
                if (tkey_comporator(node_to_split->keys[middle_element_index], parent_node->keys[middle])) { // key < current_node->keys[middle]
                    right_bound = middle - 1;
                } else if (tkey_comporator(parent_node->keys[middle], node_to_split->keys[middle_element_index])) { // key > current_node->keys[middle]
                    left_bound = middle + 1;
                } else {
                    // Can't same keys exist?
                }
            }
            for (int i = parent_node->keys_count; i > left_bound; --i) {
                parent_node->keys[i] = parent_node->keys[i - 1];
                parent_node->values[i] = std::move(parent_node->values[i - 1]);
                parent_node->sons[i + 1] = parent_node->sons[i];
            }
            parent_node->keys[left_bound] = node_to_split->keys[middle_element_index];
            parent_node->values[left_bound] = std::move(node_to_split->values[middle_element_index]);
            parent_node->sons[left_bound] = node_to_split;
            node_to_split->keys_count = _minimal_tree_power - 1;
            parent_node->sons[left_bound + 1] = new_right_node;
            ++parent_node->keys_count;

            if (is_leaf) {
                new_right_node->next = node_to_split->next;
                node_to_split->next = new_right_node;
            }
        }

    public:

        void insert(
            tkey const & key,
            tvalue && value
        ) override {
            if (_root_node == nullptr) {
                _root_node = ntools::nmalloc::malloc<bplustree_node *>(sizeof(bplustree_node));
                new (_root_node)bplustree_node(_allocator, _minimal_tree_power, true);
                _root_node->keys[0] = key;
                _root_node->values[0] = std::move(value);
                _root_node->keys_count = 1;
                _root_node->sons[0] = nullptr;
                _root_node->sons[1] = nullptr;
                _root_node->next = nullptr;
            } else {
                std::stack<bplustree_node *> nodes_stack;
                insert_inner(key, std::move(value), nodes_stack);
            }
        }

    // ===

    // Remove implementation

        tvalue remove(
            tkey const & key
        ) override {
            std::stack<bplustree_node *> nodes_stack;
            return remove_inner(key, nodes_stack);
        }

    private:

        tvalue remove_inner(
            tkey const & key,
            std::stack<bplustree_node *> & nodes_stack,
            bool first_time = true
        ) {
            // std::pair<bplustree_node *, int> inner_node_data = { nullptr, -1 };
            bool key_exists_in_inner;
            
            auto find_inner_data = find_inner(key, nodes_stack, &key_exists_in_inner, first_time);
            int element_to_delete_index = find_inner_data.first;
            
            if (find_inner_data.first == -1) {
                if (first_time) {
                    // Element does not exist and thus there is nothing to remove
                    throw std::runtime_error("No such element exception! Element does not exist and thus there is nothing to remove.");
                } else {
                    return tvalue();
                }
            }

            bplustree_node * node_to_delete = nodes_stack.top();
            tvalue data_of_deleted_element = std::move(node_to_delete->values[find_inner_data.first]);

            if (_root_node->is_leaf && _root_node->keys_count == 1) {
                _root_node->~bplustree_node();
                ntools::nmalloc::free(_root_node);
                _root_node = nullptr;
                return data_of_deleted_element;
            }

            if (nodes_stack.top()->is_leaf) {
                delete_leaf_case_one(find_inner_data.first, nodes_stack);
            } else {
                delete_inner_case(find_inner_data.first, nodes_stack);
            }

            if (key_exists_in_inner && first_time) {
                std::stack<bplustree_node *> inner_nodes_stack;
                remove_inner(key, inner_nodes_stack, false);
            }

            return data_of_deleted_element;
        }

        void delete_leaf_case_one(
            int element_to_delete_index,
            std::stack<bplustree_node *> & nodes_stack
        ) {
            bplustree_node * node = nodes_stack.top();
            for (int i = element_to_delete_index; i < node->keys_count; ++i) {
                node->keys[i] = node->keys[i + 1];
                node->values[i] = std::move(node->values[i + 1]);
            }
            --node->keys_count;
            if (node != _root_node && node->keys_count < _minimal_tree_power - 1) {
                delete_leaf_case_two(nodes_stack);
            }
        }

        std::tuple<bplustree_node *, int, bool> get_immediate_neighbour(
            std::stack<bplustree_node *> & nodes_stack,
            bool ignore_elements_limitation = false
        ) {
            if (nodes_stack.empty()) {
                return { nullptr, -1, false };
            }
            bplustree_node * current_node = nodes_stack.top();
            nodes_stack.pop();
            if (nodes_stack.empty()) {
                nodes_stack.push(current_node);
                return { nullptr, -1, false };
            }
            int i = 0;
            while (nodes_stack.top()->sons[i] != current_node) {
                ++i;
            }

            if (i != 0 && (nodes_stack.top()->sons[i - 1]->keys_count > _minimal_tree_power - 1 || ignore_elements_limitation)) {
                std::tuple<bplustree_node *, int, bool> data_to_return = { nodes_stack.top()->sons[i - 1], i - 1, true };
                nodes_stack.push(current_node);
                return data_to_return;
            } else if (i != nodes_stack.top()->keys_count && (nodes_stack.top()->sons[i + 1]->keys_count > _minimal_tree_power - 1 || ignore_elements_limitation)) {
                std::tuple<bplustree_node *, int, bool> data_to_return = { nodes_stack.top()->sons[i + 1], i, false };
                nodes_stack.push(current_node);
                return data_to_return;
            }

            nodes_stack.push(current_node);
            return { nullptr, -1, false };
        }

        void delete_leaf_case_two(
            std::stack<bplustree_node *> & nodes_stack
        ) {
            bplustree_node * node = nodes_stack.top();
            auto immediate_neightbour = get_immediate_neighbour(nodes_stack);
            
            if (std::get<0>(immediate_neightbour) == nullptr) {
                delete_leaf_case_three(nodes_stack);
                return;
            }
            
            nodes_stack.pop();
            bplustree_node * parent_node = nodes_stack.top();
            bplustree_node * node_to_borrow_from = std::get<0>(immediate_neightbour);
            int element_to_push_down_index = std::get<1>(immediate_neightbour);
            tkey borrowed_key;
            tvalue borrowed_value;
            bplustree_node * borrowed_son;
            if (std::get<2>(immediate_neightbour)) {
                // We borrow element from left neighbour
                borrowed_key = node_to_borrow_from->keys[node_to_borrow_from->keys_count - 1];
                borrowed_value = std::move(node_to_borrow_from->values[node_to_borrow_from->keys_count - 1]);
                borrowed_son = node_to_borrow_from->sons[node_to_borrow_from->keys_count];
                node_to_borrow_from->sons[node_to_borrow_from->keys_count] = nullptr;
                --node_to_borrow_from->keys_count;
                node->sons[node->keys_count + 1] = node->sons[node->keys_count];
                for (int i = node->keys_count - 1; i >= 0; --i) {
                    node->keys[i + 1] = node->keys[i];
                    node->values[i + 1] = std::move(node->values[i]);
                    node->sons[i + 1] = node->sons[i];
                }
                // node->keys[0] = parent_node->keys[element_to_push_down_index];
                // node->values[0] = std::move(parent_node->values[element_to_push_down_index]);
                
                if (node->is_leaf) {
                    node->keys[0] = borrowed_key;
                    node->values[0] = borrowed_value;
                } else {
                    node->keys[0] = node->sons[1]->keys[0];
                    node->values[0] = node->sons[1]->values[0];
                }

                node->sons[0] = borrowed_son;
                
                parent_node->keys[element_to_push_down_index] = borrowed_key;
                parent_node->values[element_to_push_down_index] = borrowed_value;
            } else {
                // We borrow element from right neighbour
                borrowed_key = node_to_borrow_from->keys[0];
                borrowed_value = std::move(node_to_borrow_from->values[0]);
                borrowed_son = node_to_borrow_from->sons[0];
                for (int i = 0; i < node_to_borrow_from->keys_count - 1; ++i) {
                    node_to_borrow_from->keys[i] = node_to_borrow_from->keys[i + 1];
                    node_to_borrow_from->values[i] = std::move(node_to_borrow_from->values[i + 1]);
                    node_to_borrow_from->sons[i] = node_to_borrow_from->sons[i + 1];
                }
                node_to_borrow_from->sons[node_to_borrow_from->keys_count - 1] = node_to_borrow_from->sons[node_to_borrow_from->keys_count];
                node_to_borrow_from->sons[node_to_borrow_from->keys_count] = nullptr;
                --node_to_borrow_from->keys_count;

                // node->keys[node->keys_count] = parent_node->keys[element_to_push_down_index];
                // node->values[node->keys_count] = std::move(parent_node->values[element_to_push_down_index]);
                
                node->sons[node->keys_count + 1] = borrowed_son;

                if (node->is_leaf) {
                    node->keys[node->keys_count] = borrowed_key;
                    node->values[node->keys_count] = borrowed_value;

                    parent_node->keys[element_to_push_down_index] = node_to_borrow_from->keys[1];
                    parent_node->values[element_to_push_down_index] = node_to_borrow_from->values[1];
                } else {
                    node->keys[node->keys_count] = borrowed_son->keys[0];
                    node->values[node->keys_count] = borrowed_son->values[0];
                    
                    parent_node->keys[element_to_push_down_index] = borrowed_key;
                    parent_node->values[element_to_push_down_index] = borrowed_value;

                    // borrowed_son->keys[borrowed_son->keys_count] = borrowed_key;
                    // borrowed_son->values[borrowed_son->keys_count] = borrowed_value;
                    // ++borrowed_son->keys_count;
                }

                // parent_node->keys[element_to_push_down_index] = node_to_borrow_from->keys[1];
                // parent_node->values[element_to_push_down_index] = node_to_borrow_from->values[1];
            }
            ++node->keys_count;
            // parent_node->keys[element_to_push_down_index] = borrowed_key;
            // parent_node->values[element_to_push_down_index] = borrowed_value;
        }

        bplustree_node * merge_nodes(
            std::stack<bplustree_node *> & nodes_stack
        ) {
            auto immediate_neighbour = get_immediate_neighbour(nodes_stack, true);

            bplustree_node * node = nodes_stack.top();
            nodes_stack.pop();
            bplustree_node * parent_node = nodes_stack.top();

            bplustree_node * node_to_merge = std::get<0>(immediate_neighbour);
            int node_to_push_down_index = std::get<1>(immediate_neighbour);

            if (!std::get<2>(immediate_neighbour)) {
                std::swap(node, node_to_merge);
            }

            node_to_merge->next = node->next;

            // We merge with left neighbour
            
            if (!node->is_leaf) {
                node_to_merge->keys[node_to_merge->keys_count] = parent_node->keys[node_to_push_down_index];
                node_to_merge->values[node_to_merge->keys_count] = std::move(parent_node->values[node_to_push_down_index]);
                ++node_to_merge->keys_count;
            }

            for (int i = 0; i < node->keys_count; ++i) {
                node_to_merge->keys[node_to_merge->keys_count] = node->keys[i];
                node_to_merge->values[node_to_merge->keys_count] = std::move(node->values[i]);
                node_to_merge->sons[node_to_merge->keys_count] = node->sons[i];
                ++node_to_merge->keys_count;
            }
            node_to_merge->sons[node_to_merge->keys_count] = node->sons[node->keys_count];

            // Now we remove pushed down node from parent node
            for (int i = node_to_push_down_index; i < parent_node->keys_count - 1; ++i) {
                parent_node->keys[i] = parent_node->keys[i + 1];
                parent_node->values[i] = std::move(parent_node->values[i + 1]);
                parent_node->sons[i] = parent_node->sons[i + 1];
            }
            parent_node->sons[parent_node->keys_count - 1] = parent_node->sons[parent_node->keys_count];
            parent_node->sons[parent_node->keys_count] = nullptr;
            --parent_node->keys_count;

            node->~bplustree_node();
            ntools::nmalloc::free(node);

            parent_node->sons[node_to_push_down_index] = node_to_merge;

            return node_to_merge;
        }

        void delete_leaf_case_three(
            std::stack<bplustree_node *> & nodes_stack
        ) {
            bplustree_node * merged_node = merge_nodes(nodes_stack);

            if (nodes_stack.size() == 1) {
                // We are in root. It can have from 1 to 2t - 1 nodes
                if (_root_node->keys_count == 0) {
                    // If there are 0 nodes in root change root
                    _root_node = merged_node;
                }
            } else {
                bplustree_node * parent_node = nodes_stack.top();
                if (parent_node->keys_count < _minimal_tree_power - 1) {
                    delete_leaf_case_two(nodes_stack);
                }
            }
        }

        void get_successor(
            int element_to_delete_index,
            std::stack<bplustree_node *> & nodes_stack
        ) {
            bplustree_node * successor_node = nodes_stack.top();
            successor_node = successor_node->sons[element_to_delete_index + 1];
            nodes_stack.push(successor_node);
            while (!successor_node->is_leaf) {
                successor_node = successor_node->sons[0];
                nodes_stack.push(successor_node);
            }
        }

        void delete_inner_case(
            int element_to_delete_index,
            std::stack<bplustree_node *> & nodes_stack
        ) {
            bplustree_node * node_to_delete = nodes_stack.top();
            get_successor(element_to_delete_index, nodes_stack);
            bplustree_node * new_node_to_delete = nodes_stack.top();
            node_to_delete->keys[element_to_delete_index] = new_node_to_delete->keys[0];
            node_to_delete->values[element_to_delete_index] = std::move(new_node_to_delete->values[0]);
            // Now our deletion is quivalent to deletion of a leaf new_node_to_delete
            // delete_leaf_case_one(0, nodes_stack);
        }

    // ===

public:

    class prefix_iterator final {

    friend prefix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::prefix_iterator_begin();
    friend prefix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::prefix_iterator_end();

    private:

        bplustree<tkey, tvalue, tkey_comporator> & _tree;
        std::stack<std::pair<bplustree_node *, int>> _traversal;
        unsigned int _height_level = 0;

        void iterator_next() {
            auto current_node_data = _traversal.top();
            if (current_node_data.second + 1 < current_node_data.first->keys_count) {
                ++_traversal.top().second;
            } else {
                if (!current_node_data.first->is_leaf) {
                    ++_height_level;
                    _traversal.top().second = 0;
                    _traversal.push({ current_node_data.first->sons[0], 0 });
                } else {
                    --_height_level;
                    _traversal.pop();
                    if (_traversal.empty()) {
                        return;
                    }
                    while (_traversal.top().second >= _traversal.top().first->keys_count) {
                        --_height_level;
                        _traversal.pop();
                        if (_traversal.empty()) {
                            return;
                        }
                    }
                    _traversal.push({ _traversal.top().first->sons[++_traversal.top().second], 0 });
                    
                }
            }
        }

        prefix_iterator(
            bplustree<tkey, tvalue, tkey_comporator> & tree,
            bool is_begin_iterator
        ) : _tree(tree) {
            if (is_begin_iterator && !_tree.empty()) {
                _traversal.push({ _tree._root_node, 0 });
            }
        }

        bool is_end_iterator() const noexcept {
            return _traversal.empty();
        }

    public:

        prefix_iterator & operator++() {
            if (is_end_iterator()) {
                return *this;
            }
            iterator_next();
            return *this;
        }

        prefix_iterator operator++(int) {
            if (is_end_iterator()) {
                return *this;
            }
            prefix_iterator previous_iterator_state = *this;
            iterator_next();
            return previous_iterator_state;
        }

        bool operator==(
            prefix_iterator const & other
        ) const {
            if (is_end_iterator() ^ other._traversal.empty()) {
                return false;
            }
            return (_traversal.empty() && other._traversal.empty()) || (_traversal.top() == other._traversal.top());
        }

        bool operator!=(
            prefix_iterator const & other
        ) const {
            return !(*this == other);
        }

        std::tuple<unsigned int, tkey, tvalue &> operator*() const {
            if (is_end_iterator()) {
                throw std::runtime_error("An attempt was made to dereference an iterator pointing to the end. But such iterator does not point to an element of the tree.");
            } else {
                return { _height_level, _traversal.top().first->keys[_traversal.top().second], _traversal.top().first->values[_traversal.top().second] };
            }
        }

    };

    prefix_iterator prefix_iterator_begin() {
        return prefix_iterator(*this, true);
    }

    prefix_iterator prefix_iterator_end() {
        return prefix_iterator(*this, false);
    }

    // B+ infix iterator (linear) implementation

    class unique_infix_iterator final {

    friend unique_infix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::uinfix_iterator_begin();
    friend unique_infix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::uinfix_iterator_end();

    private:

        bplustree<tkey, tvalue, tkey_comporator> & _tree;
        
        bplustree_node * _current_node;
        unsigned int _index = 0;

        void iterator_next() {
            if (_current_node != nullptr && ++_index == _current_node->keys_count) {
                _current_node = _current_node->next;
                _index = 0;
            }
        }

        explicit unique_infix_iterator(
            bplustree<tkey, tvalue, tkey_comporator> & tree,
            bool is_begin_iterator
        ) : _tree(tree) {
            if (is_begin_iterator && !_tree.empty()) {
                _current_node = _tree._root_node;
                while (!_current_node->is_leaf) {
                    _current_node = _current_node->sons[0];
                }
            } else {
                _current_node = nullptr;
            }
        }

        bool is_end_iterator() const noexcept {
            return _current_node == nullptr;
        }

    public:

        unique_infix_iterator & operator++() {
            if (is_end_iterator()) {
                return *this;
            }
            iterator_next();
            return *this;
        }

        unique_infix_iterator operator++(int) {
            if (is_end_iterator()) {
                return *this;
            }
            infix_iterator previous_iterator_state = *this;
            iterator_next();
            return previous_iterator_state;
        }

        bool operator==(
            unique_infix_iterator const & other
        ) const {
            return _current_node == other._current_node;
        }

        bool operator!=(
            unique_infix_iterator const & other
        ) const {
            return !(*this == other);
        }

        std::pair<tkey, tvalue &> operator*() const {
            if (is_end_iterator()) {
                throw std::runtime_error("An attempt was made to dereference an iterator pointing to the end. But such iterator does not point to an element of the tree.");
            } else {
                return { _current_node->keys[_index], _current_node->values[_index] };
            }
        }

    };

    unique_infix_iterator uinfix_iterator_begin() {
        return unique_infix_iterator(*this, true);
    }

    unique_infix_iterator uinfix_iterator_end() {
        return unique_infix_iterator(*this, false);
    }
    
    // ===

    class infix_iterator final {

    friend infix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::infix_iterator_begin();
    friend infix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::infix_iterator_end();

    private:

        bplustree<tkey, tvalue, tkey_comporator> & _tree;
        // Int is an index of array sons where we should go next
        std::stack<std::pair<bplustree_node *, int>> _traversal;
        unsigned int _height_level = 0;

        void iterator_next() {
            auto current_node_data = _traversal.top();
            if ((current_node_data.second <= current_node_data.first->keys_count) && !current_node_data.first->is_leaf) {
                ++_height_level;
                _traversal.push({ current_node_data.first->sons[current_node_data.second], 0 });
                bplustree_node * node = _traversal.top().first;
                while (!node->is_leaf) {
                    node = node->sons[0];
                    ++_height_level;
                    _traversal.push({ node, 0 });
                }
                ++_traversal.top().second;
            } else {
                if (current_node_data.first->is_leaf) {
                    ++_traversal.top().second;
                }
                while (_traversal.top().second > _traversal.top().first->keys_count) {
                    _traversal.pop();
                    --_height_level;
                    if (_traversal.empty()) {
                        return;
                    }
                    ++_traversal.top().second;
                }
            }
        }

        explicit infix_iterator(
            bplustree<tkey, tvalue, tkey_comporator> & tree,
            bool is_begin_iterator
        ) : _tree(tree) {
            if (is_begin_iterator && !_tree.empty()) {
                bplustree_node * node = _tree._root_node;
                _traversal.push({ node, 0 });
                while (!node->is_leaf) {
                    node = node->sons[0];
                    ++_height_level;
                    _traversal.push({ node, 0 });
                }
                ++_traversal.top().second;
            }
        }

        bool is_end_iterator() const noexcept {
            return _traversal.empty();
        }

    public:

        infix_iterator & operator++() {
            if (is_end_iterator()) {
                return *this;
            }
            iterator_next();
            return *this;
        }

        infix_iterator operator++(int) {
            if (is_end_iterator()) {
                return *this;
            }
            infix_iterator previous_iterator_state = *this;
            iterator_next();
            return previous_iterator_state;
        }

        bool operator==(
            infix_iterator const & other
        ) const {
            if (is_end_iterator() ^ other._traversal.empty()) {
                return false;
            }
            return (_traversal.empty() && other._traversal.empty()) || (_traversal.top() == other._traversal.top());
        }

        bool operator!=(
            infix_iterator const & other
        ) const {
            return !(*this == other);
        }

        std::tuple<unsigned int, tkey, tvalue &> operator*() const {
            if (is_end_iterator()) {
                throw std::runtime_error("An attempt was made to dereference an iterator pointing to the end. But such iterator does not point to an element of the tree.");
            } else {
                return { _height_level, _traversal.top().first->keys[_traversal.top().second - 1], _traversal.top().first->values[_traversal.top().second - 1] };
            }
        }

    };

    infix_iterator infix_iterator_begin() {
        return infix_iterator(*this, true);
    }

    infix_iterator infix_iterator_end() {
        return infix_iterator(*this, false);
    }

    class postfix_iterator final {

    friend postfix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::postfix_iterator_begin();
    friend postfix_iterator nbplustree::bplustree<tkey, tvalue, tkey_comporator>::postfix_iterator_end();

    private:

        bplustree<tkey, tvalue, tkey_comporator> & _tree;
        std::stack<std::pair<bplustree_node *, int>> _traversal;
        unsigned int _height_level = 0;
    
        void iterator_next() {
            auto current_node_data = _traversal.top();
            if (current_node_data.second + 1 < current_node_data.first->keys_count) {
                ++_traversal.top().second;
            } else {
                _traversal.pop();
                --_height_level;
                if (_traversal.empty()) {
                    return;
                }
                if (++_traversal.top().second > _traversal.top().first->keys_count) {
                    _traversal.top().second = 0;
                } else {
                    ++_height_level;
                    _traversal.push({ _traversal.top().first->sons[_traversal.top().second], 0 });
                    bplustree_node * node = _traversal.top().first;
                    while (!node->is_leaf) {
                        node = node->sons[0];
                        ++_height_level;
                        _traversal.push({ node, 0 });
                    }
                }
            }
        }

        postfix_iterator(
            bplustree<tkey, tvalue, tkey_comporator> & tree,
            bool is_begin_iterator
        ) : _tree(tree) {
            if (is_begin_iterator && !_tree.empty()) {
                bplustree_node * node = _tree._root_node;
                _traversal.push({ node, 0 });
                while (!node->is_leaf) {
                    node = node->sons[0];
                    ++_height_level;
                    _traversal.push({ node, 0 });
                }
            }
        }

        bool is_end_iterator() const noexcept {
            return _traversal.empty();
        }

    public:

        postfix_iterator & operator++() {
            if (is_end_iterator()) {
                return *this;
            }
            iterator_next();
            return *this;
        }

        postfix_iterator operator++(int) {
            if (is_end_iterator()) {
                return *this;
            }
            postfix_iterator previous_iterator_state = *this;
            iterator_next();
            return previous_iterator_state;
        }

        bool operator==(
            postfix_iterator const & other
        ) const {
            if (is_end_iterator() ^ other._traversal.empty()) {
                return false;
            }
            return (_traversal.empty() && other._traversal.empty()) || (_traversal.top() == other._traversal.top());
        }

        bool operator!=(
            postfix_iterator const & other
        ) const {
            return !(*this == other);
        }

        std::tuple<unsigned int, tkey, tvalue &> operator*() const {
            if (is_end_iterator()) {
                throw std::runtime_error("An attempt was made to dereference an iterator pointing to the end. But such iterator does not point to an element of the tree.");
            } else {
                return { _height_level, _traversal.top().first->keys[_traversal.top().second], _traversal.top().first->values[_traversal.top().second] };
            }
        }

    };

    postfix_iterator postfix_iterator_begin() {
        return postfix_iterator(*this, true);
    }

    postfix_iterator postfix_iterator_end() {
        return postfix_iterator(*this, false);
    }

    // Print implementation

        void print(
            bool to_logger,
            ...
        ) noexcept {
            nlogger::logger * logger = nullptr;
            if (to_logger) {
                std::va_list args;
                va_start(args, to_logger);
                logger = va_arg(args, nlogger::logger *);
            }
            print_inner(logger, _root_node, 0);
        }

    private:

        void print_inner(
            nlogger::logger * logger,
            bplustree_node * node,
            int level = 0
        ) noexcept {
            if (node == nullptr) {
                return;
            }
            std::string to_output = "Level " + std::to_string(level) + ": ";
            for (int i = 0; i < node->keys_count; ++i) {
                to_output += std::to_string(node->keys[i]) + " ";
            }
            if (logger != nullptr) {
                logger->log(to_output, nlogger::logger::severity::information);
            } else {
                std::cout << to_output << std::endl;
            }
            level += 1;
            if (!node->is_leaf) {
                for (int i = 0; i <= node->keys_count; ++i) {
                    if (node->sons[i] != nullptr) {
                        print_inner(logger, node->sons[i], level);
                    }
                }
            }
        }

        // ===

    };

} /* nbtree */

#endif /* BPLUSTREE */

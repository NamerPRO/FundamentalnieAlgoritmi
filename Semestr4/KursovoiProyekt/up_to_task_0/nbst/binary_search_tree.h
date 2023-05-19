#ifndef TASK11_BINARY_SEARCH_TREE_H
#define TASK11_BINARY_SEARCH_TREE_H

#include <iostream>
#include <memory>
#include <new>
#include <stack>
#include <tuple>
#include <type_traits>

#include "associative_container.h"
#include "./nmalloc.h"
#include "../nmemory/memory.h"

namespace nbst {

    class standard_comporators final {

    public:

        template<
                typename T,
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
                >
        static bool basic_comporator(
                T const & lhs,
                T const & rhs
                ) {
            return lhs < rhs;
        }

//        static bool string_comporator(
//                std::string const & lhs,
//                std::string const & rhs
//                ) {
//            for (int i = 0; i < lhs.size(); ++i) {
//                if (lhs[i] == rhs[i]) {
//                    continue;
//                }
//                if (lhs[i] < rhs[i]) {
//                    return true;
//                }
//                return false;
//            }
//            return false;
//        }

    };

    template<typename tkey, typename tvalue, bool (*tkey_comporator)(tkey const &, tkey const &) = standard_comporators::basic_comporator>
    class binary_search_tree : protected ntools::nmalloc, public ncontainer::associative_container<tkey, tvalue, tkey_comporator> {

        protected:

            struct tree_node {

                tkey key;
                tvalue value;
                tree_node * left_child;
                tree_node * right_child;

                virtual ~tree_node() noexcept = default;

            };

            tree_node * _root_node;

        private:

            void clone(binary_search_tree const & tree) {
                prefix_iterator it_begin = const_cast<binary_search_tree &>(tree).prefix_iterator_begin();
                prefix_iterator it_end = const_cast<binary_search_tree &>(tree).prefix_iterator_end();
                while (it_begin != it_end) {
                    std::tuple<unsigned int, tkey, tvalue> node_data = *it_begin;
                    insert(std::get<1>(node_data), std::move(std::get<2>(node_data)));
                    ++it_begin;
                }
                _allocator = tree._allocator;
            }

            void clear() {
                while (!empty()) {
                    remove(_root_node->key);
                }
            }

        public:

            explicit binary_search_tree(
                    nmemory::memory * allocator = nullptr
                    ) noexcept : ntools::nmalloc(allocator) {
                _root_node = nullptr;
            }

            binary_search_tree(
                    binary_search_tree<tkey, tvalue, tkey_comporator> const & tree
                    ) noexcept : ntools::nmalloc(tree._allocator) {
                _root_node = nullptr;
                clone(tree);
            }

            binary_search_tree<tkey, tvalue, tkey_comporator> & operator=(
                    binary_search_tree<tkey, tvalue, tkey_comporator> const & tree
                    ) noexcept {
                if (&tree == this) {
                    return *this;
                }
                clear();
                clone(tree);
                return *this;
            }

            binary_search_tree(
                    binary_search_tree<tkey, tvalue, tkey_comporator> && tree
                    ) noexcept : ntools::nmalloc(tree._allocator) {
                _root_node = tree._root_node;
                tree._root_node = nullptr;
                tree._allocator = nullptr;
            }

            binary_search_tree<tkey, tvalue, tkey_comporator> & operator=(
                    binary_search_tree<tkey, tvalue, tkey_comporator> && tree
                    ) noexcept {
                if (&tree == this) {
                    return *this;
                }
                clear();
                _root_node = tree._root_node;
                _allocator = tree._allocator;
                tree._root_node = nullptr;
                tree._allocator = nullptr;
                return *this;
            }

            ~binary_search_tree() noexcept {
                clear();
            }

        // Insert implementation:
        public:

            void insert(
                    tkey const & key,
                    tvalue && value
            ) override {
                std::stack<tree_node *> nodes_stack;
                insert_inner(key, std::move(value), nodes_stack);
            }

        protected:

            virtual void initialize_memory(tree_node * node_ptr) {
                new (node_ptr)tree_node;
            }

            virtual unsigned int get_node_size() noexcept {
                return sizeof(tree_node);
            }

        private:

            void insert_inner(
                    tkey const & key,
                    tvalue && value,
                    std::stack<tree_node *> & nodes_stack
            ) {
                before_element_inserted(_root_node, nodes_stack);

                if (_root_node == nullptr) {
                    _root_node = ntools::nmalloc::malloc<tree_node *>(get_node_size());

                    initialize_memory(_root_node);

                    _root_node->left_child = nullptr;
                    _root_node->right_child = nullptr;
                    _root_node->value = std::move(value);
                    _root_node->key = key;

                    nodes_stack.push(_root_node);
                    
                    after_element_inserted(nodes_stack);
                } else {
                    find_inner(key, _root_node, nodes_stack, false);

                    if (nodes_stack.top() != nullptr) {
                        on_insertable_element_exists(key, std::move(value), nodes_stack.top(), nodes_stack);
                    } else {
                        nodes_stack.pop();
                        tree_node * parent_node = nodes_stack.top();
                        tree_node * child_node = insert_at_position(parent_node, key, std::move(value), tkey_comporator(key, parent_node->key));
                        nodes_stack.push(child_node);
                        after_element_inserted(nodes_stack);
                    }
                }
            }

        protected:

            virtual tree_node * insert_at_position(
                    tree_node * parent_node,
                    tkey const & key,
                    tvalue && value,
                    bool is_left_child
            ) noexcept {
                if (is_left_child) {
                    parent_node->left_child = ntools::nmalloc::malloc<tree_node *>(get_node_size()/*sizeof(tree_node)*/);
                    initialize_memory(parent_node->left_child);
                    parent_node->left_child->key = key;
                    parent_node->left_child->value = std::move(value);
                    parent_node->left_child->left_child = nullptr;
                    parent_node->left_child->right_child = nullptr;
                    return parent_node->left_child;
                } else {
                    parent_node->right_child = ntools::nmalloc::malloc<tree_node *>(get_node_size()/*sizeof(tree_node)*/);
                    initialize_memory(parent_node->right_child);
                    parent_node->right_child->key = key;
                    parent_node->right_child->value = std::move(value);
                    parent_node->right_child->left_child = nullptr;
                    parent_node->right_child->right_child = nullptr;
                    return parent_node->right_child;
                }
            }

            virtual void after_element_inserted(
                    std::stack<tree_node *> & nodes_stack
            ) noexcept {
                // ToDo("Possible to override...")
            }

            virtual void before_element_inserted(
                    tree_node * root_node,
    //                    tkey const & key,
                    std::stack<tree_node *> & nodes_stack
            ) noexcept {
                // ToDo("Possible to override...")
            }

            virtual void on_insertable_element_exists(
                    tkey const & key,
                    tvalue && value,
                    tree_node * found_node,
                    std::stack<tree_node *> & nodes_stack
            ) noexcept {
                found_node->key = key;
                found_node->value = std::move(value);
            }
         // ===

        // Find impementation
        public:

            tvalue & find(
                    tkey const & key
            ) override {
                std::stack<tree_node *> nodes_stack;
                tvalue * value_to_return_ptr = find_inner(key, _root_node, nodes_stack);
                if (value_to_return_ptr == nullptr) {
                    throw std::runtime_error("No such element exception! Could not find requested element by key.");
                }
                return *value_to_return_ptr;
            }

        private:

            tvalue * find_inner(
                    tkey const & key,
                    tree_node * root_node,
                    std::stack<tree_node *> & nodes_stack,
                    bool call_on_element_found_function = true
            ) noexcept {
                before_element_found(key, root_node, nodes_stack);

                tree_node * node_to_find = _root_node;
                while (node_to_find != nullptr) {
                    nodes_stack.push(node_to_find);
                    if (tkey_comporator(key, node_to_find->key)) {
                        node_to_find = node_to_find->left_child;
                    } else if (tkey_comporator(node_to_find->key, key)) {
                        node_to_find = node_to_find->right_child;
                    } else {
                        break;
                    }
                }

                if (node_to_find == nullptr) {
                    nodes_stack.push(nullptr);
                    after_element_not_found(key, nullptr, nodes_stack);
                    return nullptr;
                } else {
                    if (call_on_element_found_function) {
                        after_element_found(key, node_to_find, nodes_stack);
                    }
                    return &(node_to_find->value);
                }

            }

        protected:

            virtual void before_element_found(
                    tkey const & key,
                    tree_node * root_node,
                    std::stack<tree_node *> & nodes_stack
            ) noexcept {
                // ToDo("Possible to override...")
            }

            virtual void after_element_found(
                    tkey const & key,
                    tree_node * found_node,
                    std::stack<tree_node *> & nodes_stack
            ) noexcept {
                // ToDo("Possible to override...")
            }

            virtual void after_element_not_found(
                    tkey const & key,
                    tree_node * found_node,
                    std::stack<tree_node *> & nodes_stack
            ) noexcept {
                // ToDo("Possible to override...")
            }
            // ===

        // Is empty implementation
        public:

            bool empty() override {
                return _root_node == nullptr;
            }
        // ===

        // Remove implementation
        public:

            tvalue remove(
                    tkey const & key
            ) override {
                std::stack<tree_node *> nodes_stack;
                return remove_inner(key, nodes_stack);
            }

        private:

            tvalue remove_inner(
                    tkey const & key,
                    std::stack<tree_node *> & nodes_stack
            ) {
                before_element_removed(key, _root_node, nodes_stack);

                if (find_inner(key, _root_node, nodes_stack, false) == nullptr) {
                    throw std::runtime_error("No such element exception! An attempt was made to delete an element that does not exist.");
                }

                tvalue element_to_remove_data = remove_element_from_tree(nodes_stack);

                after_element_removed(key, nodes_stack);

                return element_to_remove_data;
            }

        protected:

            virtual void before_element_removed(
                    tkey const & key,
                    tree_node * root_node,
                    std::stack<tree_node *> & nodes_stack
                    ) noexcept {
                // ToDo("Called right when removal process started. Stack is empty.");
            }

//            virtual void right_before_element_removed(
//                    tkey const & key,
//                    std::stack<tree_node *> & nodes_stack
//                    ) {
//                // ToDo("Called right before deletion of an element. Element to delete is on stack top.");
//            }

            virtual tvalue remove_element_from_tree(
                    std::stack<tree_node *> & nodes_stack
                    ) {
                tree_node * element_to_remove_ptr = nodes_stack.top();
                //            nodes_stack.pop();

                tvalue element_to_remove_data = std::move(element_to_remove_ptr->value);

                if (element_to_remove_ptr->left_child == nullptr
                    && element_to_remove_ptr->right_child == nullptr) {
//                    right_before_element_removed(key, nodes_stack);
                    nodes_stack.pop();
                    if (nodes_stack.empty()) {
                        _root_node = nullptr;
                    } else {
                        tree_node * parent_node = nodes_stack.top();
                        if (parent_node->left_child == element_to_remove_ptr) {
                            parent_node->left_child = nullptr;
                        } else {
                            parent_node->right_child = nullptr;
                        }
                    }
                    element_to_remove_ptr->~tree_node();
                    ntools::nmalloc::free(element_to_remove_ptr);
                } else if (element_to_remove_ptr->left_child != nullptr
                           && element_to_remove_ptr->right_child != nullptr) {
                    tree_node * next_node = element_to_remove_ptr->right_child;
                    nodes_stack.push(next_node);
                    while (next_node->left_child != nullptr) {
                        next_node = next_node->left_child;
                        nodes_stack.push(next_node);
                    }
                    element_to_remove_ptr->value = std::move(next_node->value); // next_node->value теперь имеет значение по умолчанию
                    element_to_remove_ptr->key = next_node->key;
//                    right_before_element_removed(key, nodes_stack);
                    nodes_stack.pop(); // =====
                    tree_node * next_node_parent_node = nodes_stack.top();
                    if (next_node_parent_node->left_child == next_node) {
                        next_node_parent_node->left_child = next_node->right_child;
                    } else {
                        next_node_parent_node->right_child = next_node->right_child;
                    }
                    next_node->~tree_node();
                    ntools::nmalloc::free(next_node);
                } else {
//                    right_before_element_removed(key, nodes_stack);
                    nodes_stack.pop();
                    if (nodes_stack.empty()) {
                        if (element_to_remove_ptr->left_child != nullptr) {
                            _root_node = element_to_remove_ptr->left_child;
                        } else {
                            _root_node = element_to_remove_ptr->right_child;
                        }
                    } else {
                        tree_node * parent_node = nodes_stack.top();
                        if (parent_node->left_child == element_to_remove_ptr) {
                            if (element_to_remove_ptr->left_child != nullptr) {
                                parent_node->left_child = element_to_remove_ptr->left_child;
                            } else {
                                parent_node->left_child = element_to_remove_ptr->right_child;
                            }
                        } else {
                            if (element_to_remove_ptr->left_child != nullptr) {
                                parent_node->right_child = element_to_remove_ptr->left_child;
                            } else {
                                parent_node->right_child = element_to_remove_ptr->right_child;
                            }
                        }
                    }
                    element_to_remove_ptr->~tree_node();
                    ntools::nmalloc::free(element_to_remove_ptr);
                }

                return element_to_remove_data;
            }


            virtual void after_element_removed(
                    tkey const & key,
                    std::stack<tree_node *> & nodes_stack
                    ) noexcept {
                // ToDo("Called after element removed. Stack contains path to removed element with its parent on top.");
            }
        // ===

            // ===

        public:

            class prefix_iterator final {

            private:

                friend prefix_iterator nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::prefix_iterator_begin();
                friend prefix_iterator nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::prefix_iterator_end();

                explicit prefix_iterator(
                        binary_search_tree<tkey, tvalue, tkey_comporator> const & tree,
                        bool is_begin_iterator
                        ) noexcept : _tree(tree) {
                    if (is_begin_iterator) {
                        _traversal.push(_tree._root_node);
                    } else {
                        _traversal.push(nullptr);
                    }
                }

                void iterator_next() noexcept {
                    tree_node * node_to_return = _traversal.top();
                    if (node_to_return->left_child != nullptr) {
                        _traversal.push(node_to_return->left_child);
                        ++_height_level;
                    } else if (node_to_return->right_child != nullptr) {
                        _traversal.push(node_to_return->right_child);
                        ++_height_level;
                    } else {
                        tree_node * child_node = node_to_return;
                        while (true) {
                            --_height_level;
                            _traversal.pop();
                            if (_traversal.empty()) {
                                _traversal.push(nullptr);
                                break;
                            }
                            tree_node * parent_node = _traversal.top();
                            if (child_node == parent_node->left_child) {
                                if (parent_node->right_child != nullptr) {
                                    ++_height_level;
                                    _traversal.push(parent_node->right_child);
                                    break;
                                }
                            }
                            child_node = parent_node;
                        }
                    }
                }

                bool is_end_iterator() const noexcept {
                    return _traversal.top() == nullptr;
                }

                binary_search_tree<tkey, tvalue, tkey_comporator> const & _tree;
                std::stack<tree_node *> _traversal;
                unsigned int _height_level = 0;

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
                    nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::prefix_iterator previous_state_iterator_copy = *this;
                    iterator_next();
                    return previous_state_iterator_copy;
                }

                bool operator==(prefix_iterator const & other) const {
                    return _traversal.top() == other._traversal.top();
                }

                bool operator!=(prefix_iterator const & other) const {
                    return _traversal.top() != other._traversal.top();
                }

                std::tuple<unsigned int, tkey, tvalue> operator*() const {
                    if (is_end_iterator()) {
                        throw std::runtime_error("An attempt was made to dereference an iterator pointing to the end. But such iterator does not point to an element of the tree.");
                    } else {
                        return { _height_level, _traversal.top()->key, _traversal.top()->value };
                    }
                }

            };

            prefix_iterator prefix_iterator_begin() {
                return prefix_iterator(*this, true);
            }

            prefix_iterator prefix_iterator_end() {
                return prefix_iterator(*this, false);
            }

            class infix_iterator final{

            private:

                friend infix_iterator nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::infix_iterator_begin();
                friend infix_iterator nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::infix_iterator_end();

                explicit infix_iterator(
                        binary_search_tree<tkey, tvalue, tkey_comporator> & tree,
                        bool is_begin_iterator
                        ) noexcept : _tree(tree) {
                    if (is_begin_iterator && !_tree.empty()) {
                        tree_node * node = _tree._root_node;
                        _traversal.push(node);
                        while (node->left_child != nullptr) {
                            node = node->left_child;
                            ++_height_level;
                            _traversal.push(node);
                        }
    //                    _traversal.push(_tree._root_node);
                    } else {
                        _traversal.push(nullptr);
                    }
                }

                binary_search_tree<tkey, tvalue, tkey_comporator> & _tree;
                std::stack<tree_node *> _traversal;
                unsigned int _height_level = 0;

                void iterator_next() {
                    tree_node * node_to_return = _traversal.top();
                    tree_node * next_node = node_to_return->right_child;
                    if (next_node != nullptr) {
                        _traversal.push(next_node);
                        ++_height_level;
                        while (next_node->left_child != nullptr) {
                            next_node = next_node->left_child;
                            ++_height_level;
                            _traversal.push(next_node);
                        }
                    } else {
                        --_height_level;
                        _traversal.pop();
                        if (_traversal.empty()) {
                            _traversal.push(nullptr);
                            return /*previous_iterator_state_copy*/;
    //                        return node_to_return;
                        }
                        // if parent_node's right child pop stack
                        tree_node * current_node = node_to_return;
                        tree_node * parent_node = _traversal.top();
                        while (parent_node->right_child == current_node) {
                            --_height_level;
                            _traversal.pop();
                            if (_traversal.empty()) {
                                _traversal.push(nullptr);
                                break;
                            }
                            current_node = parent_node;
                            parent_node = _traversal.top();
                        }
                    }
                }

                bool is_end_iterator() const noexcept {
                    return _traversal.top() == nullptr;
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
                    nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::infix_iterator previous_iterator_state_copy = *this;
                    iterator_next();
                    return previous_iterator_state_copy;
                }

                bool operator==(infix_iterator const & other) const {
                    return _traversal.top() == other._traversal.top();
                }

                bool operator!=(infix_iterator const & other) const {
                    return _traversal.top() != other._traversal.top();
                }

                std::tuple<unsigned int, tkey, tvalue> operator*() const {
                    if (is_end_iterator()) {
                        throw std::runtime_error("An attempt was made to dereference an iterator pointing to the end. But such iterator does not point to an element of the tree.");
                    } else {
                        return { _height_level, _traversal.top()->key, _traversal.top()->value };
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

            private:

                friend postfix_iterator nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::postfix_iterator_begin();
                friend postfix_iterator nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::postfix_iterator_end();

                binary_search_tree<tkey, tvalue, tkey_comporator> & _tree;
                std::stack<tree_node *> _traversal;
                unsigned int _height_level = 0;

                explicit postfix_iterator(
                        binary_search_tree<tkey, tvalue, tkey_comporator> & tree,
                        bool is_begin_iterator
                        ) noexcept : _tree(tree) {
                    if (is_begin_iterator && !_tree.empty()) {
                        tree_node * node = _tree._root_node;
                        _traversal.push(node);
                        while (true) {
                            while (node->left_child != nullptr) {
                                node = node->left_child;
                                ++_height_level;
                                _traversal.push(node);
                            }
                            if (node->right_child == nullptr) {
                                break;
                            }
                            node = node->right_child;
                            ++_height_level;
                            _traversal.push(node);
                        }
                    } else {
                        _traversal.push(nullptr);
                    }
                }

                void iterator_next() {
                    tree_node * node_to_return = _traversal.top();

                    _traversal.pop();

                    if (_traversal.empty()) {
                        _traversal.push(nullptr);
                        return;
    //                    return node_to_return;
                    }

                    --_height_level;
                    tree_node * next_node = _traversal.top();
                    if (next_node->right_child == nullptr ||
                        node_to_return == next_node->right_child) {
                        return /*node_to_return*/;
                    }
                    next_node = next_node->right_child;
                    ++_height_level;
                    _traversal.push(next_node);

                    while (true) {
                        while (next_node->left_child != nullptr) {
                            next_node = next_node->left_child;
                            ++_height_level;
                            _traversal.push(next_node);
                        }
                        if (next_node->right_child == nullptr) {
                            break;
                        }
                        next_node = next_node->right_child;
                        ++_height_level;
                        _traversal.push(next_node);
                    }
                }

                bool is_end_iterator() const noexcept {
                    return _traversal.top() == nullptr;
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
                    nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::postfix_iterator previous_iterator_state_copy = *this;
                    iterator_next();
                    return previous_iterator_state_copy;
                }

                bool operator==(postfix_iterator const & other) const {
                    return _traversal.top() == other._traversal.top();
                }

                bool operator!=(postfix_iterator const & other) const {
                    return _traversal.top() != other._traversal.top();
                }
                // вернуть const &
                std::tuple<unsigned int, tkey, tvalue> operator*() const {
                    if (is_end_iterator()) {
                        throw std::runtime_error("An attempt was made to dereference an iterator pointing to the end. But such iterator does not point to an element of the tree.");
                    } else {
                        return { _height_level, _traversal.top()->key, _traversal.top()->value };
                    }
                }

            };

            postfix_iterator postfix_iterator_begin() {
                return postfix_iterator(*this, true);
            }

            postfix_iterator postfix_iterator_end() {
                return postfix_iterator(*this, false);
            }

        // Tree rotate implementation
        protected:

            tree_node * rotate_left(
                    tree_node * rotate_root
                    ) noexcept {
                before_left_rotate(rotate_root);

                tree_node * saved_right_child = rotate_root->right_child;
                rotate_root->right_child = saved_right_child->left_child;
                saved_right_child->left_child = rotate_root;

                if (_root_node == rotate_root) {
                    _root_node = saved_right_child;
                }

                after_left_rotate(rotate_root, saved_right_child);

                return saved_right_child;
            }

            virtual void before_left_rotate(
                    tree_node * rotate_node
                    ) noexcept {

            }

            virtual void after_left_rotate(
                    tree_node * rotate_node,
                    tree_node * saved_right_child
                    ) noexcept {

            }

            tree_node * rotate_right(
                    tree_node * rotate_root
                    ) noexcept {
                before_right_rotate(rotate_root);

                tree_node * saved_left_child = rotate_root->left_child;
                rotate_root->left_child = saved_left_child->right_child;
                saved_left_child->right_child = rotate_root;

                if (_root_node == rotate_root) {
                    _root_node = saved_left_child;
                }

                after_right_rotate(rotate_root, saved_left_child);

                return saved_left_child;
            }

            virtual void before_right_rotate(
                    tree_node * rotate_node
                    ) noexcept {

            }

            virtual void after_right_rotate(
                    tree_node * rotate_node,
                    tree_node * saved_left_child
                    ) noexcept {

            }
        // ===

        };

} // nbst

#endif //TASK11_BINARY_SEARCH_TREE_H

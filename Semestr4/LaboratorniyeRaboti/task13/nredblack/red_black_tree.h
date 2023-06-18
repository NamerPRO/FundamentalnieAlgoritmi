#ifndef TASK13_RED_BLACK_TREE_H
#define TASK13_RED_BLACK_TREE_H

#include "../nbst/binary_search_tree.h"
#include "../nbst/associative_container.h"
#include <cstddef>
#include <stack>

namespace nredblack {

    template<typename tkey, typename tvalue, bool (*tkey_comporator)(tkey const &, tkey const &) = nbst::standard_comporators::basic_comporator>
    class red_black_tree : public nbst::binary_search_tree<tkey, tvalue, tkey_comporator> {

    public:

        explicit red_black_tree(
            nmemory::memory * allocator
        ) noexcept {
            bst::_allocator = allocator;
            bst::_root_node = nullptr;
        }

        explicit red_black_tree() noexcept : red_black_tree(nullptr) {}

        red_black_tree(
            red_black_tree const & tree
        ) noexcept {
            bst::_allocator = tree._allocator;
            bst::_root_node = nullptr;
            bst::clone(tree);
        }

        red_black_tree & operator=(
            red_black_tree const & tree
        ) noexcept {
            if (&tree == this) {
                return *this;
            }
            bst::clear();
            bst::clone(tree);
            return *this;
        }

        red_black_tree(
            red_black_tree && tree
        ) noexcept {
            bst::_root_node = tree._root_node;
            bst::_allocator = tree._allocator;
            tree._root_node = nullptr;
            tree._allocator = nullptr;
        }

        red_black_tree & operator=(
            red_black_tree && tree
        ) noexcept {
            if (&tree == this) {
                return *this;
            }
            bst::clear();
            bst::_root_node = tree._root_node;
            bst::_allocator = tree._allocator;
            tree._root_node = nullptr;
            tree._allocator = nullptr;
            return *this;
        }

        ~red_black_tree() noexcept {
            bst::clear();
        }
    
    private:

        enum class node_color {
            clRed,
            clBlack
        };

        using bst = nbst::binary_search_tree<tkey, tvalue, tkey_comporator>;

        struct red_black_node : public bst::tree_node {

            node_color color;

        };

        unsigned int get_node_size() noexcept override {
            return sizeof(red_black_node);
        }

        red_black_node * to_red_black_node(
                typename bst::tree_node * node
                ) noexcept {
            return reinterpret_cast<red_black_node *>(node);
        }

        std::tuple<typename bst::tree_node *, typename bst::tree_node *, typename bst::tree_node *> get_relatives(
                std::stack<typename bst::tree_node *> & nodes_stack
                ) noexcept {
            if (nodes_stack.empty()) {
                return { nullptr, nullptr, nullptr };
            }
            typename bst::tree_node * current_node = nodes_stack.top();
            nodes_stack.pop();
            if (nodes_stack.empty()) {
                nodes_stack.push(current_node);
                return { nullptr, nullptr, nullptr };
            }
            typename bst::tree_node * parent_node = nodes_stack.top();
            nodes_stack.pop();
            if (nodes_stack.empty()) {
                nodes_stack.push(parent_node);
                nodes_stack.push(current_node);
                return { parent_node, nullptr, nullptr };
            }
            typename bst::tree_node * grandparent_node = nodes_stack.top();
            nodes_stack.pop();
            if (nodes_stack.empty()) {
                nodes_stack.push(grandparent_node);
                nodes_stack.push(parent_node);
                nodes_stack.push(current_node);
                return { parent_node, grandparent_node, nullptr };
            }
            typename bst::tree_node * greatparent_node = nodes_stack.top();
            nodes_stack.push(grandparent_node);
            nodes_stack.push(parent_node);
            nodes_stack.push(current_node);
            return { parent_node, grandparent_node, greatparent_node };
        }

        typename bst::tree_node * get_uncle(
                typename bst::tree_node * parent_node,
                typename bst::tree_node * grandparent_node
                ) noexcept {
            if (grandparent_node == nullptr) {
                return nullptr;
            }
            if (parent_node == grandparent_node->left_child) {
                return grandparent_node->right_child;
            } else {
                return grandparent_node->left_child;
            }
        }

        typename bst::tree_node * get_brother(
            typename bst::tree_node * current_node,
            typename bst::tree_node * parent_node
        ) noexcept {
            if (parent_node == nullptr) {
                return nullptr;
            }

            if (current_node == parent_node->left_child) {
                return parent_node->right_child;
            } else {
                return parent_node->left_child;
            }
        }

        node_color get_color(
            typename bst::tree_node * node
        ) {
            if (node == nullptr) {
                return node_color::clBlack;
            }
            return to_red_black_node(node)->color;
        }

        // Insert implementation

        void fix_insertion(
            std::stack<typename bst::tree_node *> & nodes_stack
        ) {
            auto relatives_nodes = get_relatives(nodes_stack);
            red_black_node * current_node = to_red_black_node(nodes_stack.top());
            red_black_node * parent_node = to_red_black_node(std::get<0>(relatives_nodes));
            red_black_node * grandparent_node = to_red_black_node(std::get<1>(relatives_nodes));
            red_black_node * greatparent_node = to_red_black_node(std::get<2>(relatives_nodes));
            red_black_node * uncle_node = to_red_black_node(get_uncle(parent_node, grandparent_node));

            while (get_color(parent_node) == node_color::clRed) {

                if (parent_node == grandparent_node->left_child) {
                    if (get_color(uncle_node) == node_color::clRed) {
                        
                        parent_node->color = node_color::clBlack;
                        uncle_node->color = node_color::clBlack;
                        grandparent_node->color = node_color::clRed;

                        nodes_stack.pop();
                        nodes_stack.pop();
                    
                    } else {
                        if (parent_node->right_child == current_node) {
                            nodes_stack.pop();
                            nodes_stack.pop();

                            nodes_stack.push(current_node);
                            nodes_stack.push(parent_node);

                            bst::rotate_left(parent_node, grandparent_node);

                            parent_node = current_node;                        
                        }
                        parent_node->color = node_color::clBlack;
                        grandparent_node->color = node_color::clRed;
                        bst::rotate_right(grandparent_node, greatparent_node);
                    }
                } else {
                    if (get_color(uncle_node) == node_color::clRed) {
                        parent_node->color = node_color::clBlack;
                        uncle_node->color = node_color::clBlack;
                        grandparent_node->color = node_color::clRed;

                        nodes_stack.pop();
                        nodes_stack.pop();
                    } else {
                        if (parent_node->left_child == current_node) {
                            nodes_stack.pop();
                            nodes_stack.pop();

                            nodes_stack.push(current_node);
                            nodes_stack.push(parent_node);

                            bst::rotate_right(parent_node, grandparent_node);

                            parent_node = current_node;
                        }
                        parent_node->color = node_color::clBlack;
                        grandparent_node->color = node_color::clRed;
                        bst::rotate_left(grandparent_node, greatparent_node);
                    }
                }

                relatives_nodes = get_relatives(nodes_stack);
                current_node = to_red_black_node(nodes_stack.top());
                parent_node = to_red_black_node(std::get<0>(relatives_nodes));
                grandparent_node = to_red_black_node(std::get<1>(relatives_nodes));
                greatparent_node = to_red_black_node(std::get<2>(relatives_nodes));
                uncle_node = to_red_black_node(get_uncle(parent_node, grandparent_node));
            }

            to_red_black_node(bst::_root_node)->color = node_color::clBlack;
        }

        void after_element_inserted(
            std::stack<typename bst::tree_node *> & nodes_stack
        ) noexcept override {
            to_red_black_node(nodes_stack.top())->color = node_color::clRed;
            if (nodes_stack.top() == bst::_root_node) {
                to_red_black_node(bst::_root_node)->color = node_color::clBlack;
                return;
            }
            if (get_color(std::get<0>(get_relatives(nodes_stack))) == node_color::clRed) {
                fix_insertion(nodes_stack);
            }
        }

        // ===

        // Remove implementation

        void update_relatives(
            std::stack<typename bst::tree_node *> & nodes_stack,
            typename bst::tree_node ** current_node,
            red_black_node ** parent_node,
            red_black_node ** grandparent_node,
            red_black_node ** brother_node
        ) {
            auto relative_nodes = get_relatives(nodes_stack);
            *current_node = to_red_black_node(nodes_stack.top());
            *parent_node = to_red_black_node(std::get<0>(relative_nodes));
            *grandparent_node = to_red_black_node(std::get<1>(relative_nodes));
            *brother_node = to_red_black_node(get_brother(*current_node, *parent_node));
        }

        void fix_removal(
            std::stack<typename bst::tree_node *> & nodes_stack,
            typename bst::tree_node * instead_of_removed_ptr
        ) {
            auto relative_nodes = get_relatives(nodes_stack);
            red_black_node * parent_node = to_red_black_node(std::get<0>(relative_nodes));
            red_black_node * grandparent_node = to_red_black_node(std::get<1>(relative_nodes));
            red_black_node * brother_node = to_red_black_node(get_brother(instead_of_removed_ptr, parent_node));

            while (get_color(nodes_stack.top()) == node_color::clBlack && parent_node != nullptr) {
                if (instead_of_removed_ptr == parent_node->left_child) {
                    if (get_color(brother_node) == node_color::clRed) {
                        brother_node->color = node_color::clBlack;
                        parent_node->color = node_color::clRed;
                        
                        bst::rotate_left(parent_node, grandparent_node, &nodes_stack, 1);

                        update_relatives(nodes_stack, &instead_of_removed_ptr, &parent_node, &grandparent_node, &brother_node);

                    }
                    if (get_color(brother_node->left_child) == node_color::clBlack &&
                            get_color(brother_node->right_child) == node_color::clBlack) {
                        brother_node->color = node_color::clRed;
                        
                        nodes_stack.pop();
                    } else {
                        if (get_color(brother_node->right_child) == node_color::clBlack) {
                            if (brother_node->left_child != nullptr) {
                                to_red_black_node(brother_node->left_child)->color = node_color::clBlack;
                            }
                            brother_node->color = node_color::clRed;
                            
                            bst::rotate_right(brother_node, parent_node);

                            update_relatives(nodes_stack, &instead_of_removed_ptr, &parent_node, &grandparent_node, &brother_node);
                        }
                        brother_node->color = parent_node->color;
                        parent_node->color = node_color::clBlack;

                        if (brother_node->right_child != nullptr) {
                            to_red_black_node(brother_node->right_child)->color = node_color::clBlack;
                        }

                        bst::rotate_left(parent_node, grandparent_node, &nodes_stack, 1);
                        
                        instead_of_removed_ptr = to_red_black_node(bst::_root_node);
                        break;
                    }
                } else {
                    if (get_color(brother_node) == node_color::clRed) {
                        brother_node->color = node_color::clBlack;
                        parent_node->color = node_color::clRed;

                        bst::rotate_right(parent_node, grandparent_node, &nodes_stack, 1);

                        update_relatives(nodes_stack, &instead_of_removed_ptr, &parent_node, &grandparent_node, &brother_node);
                    }

                    if (get_color(brother_node->left_child) == node_color::clBlack &&
                            get_color(brother_node->right_child) == node_color::clBlack) {
                        brother_node->color = node_color::clRed;
                        
                        nodes_stack.pop();
                    } else {
                        if (get_color(brother_node->left_child) == node_color::clBlack) {

                            if (brother_node->right_child != nullptr) {
                                to_red_black_node(brother_node->right_child)->color = node_color::clBlack;
                            }
                            brother_node->color = node_color::clRed;

                            bst::rotate_left(brother_node, parent_node);

                            update_relatives(nodes_stack, &instead_of_removed_ptr, &parent_node, &grandparent_node, &brother_node);
                        }
                        brother_node->color = parent_node->color;
                        parent_node->color = node_color::clBlack;

                        if (brother_node->left_child != nullptr) {
                            to_red_black_node(brother_node->left_child)->color = node_color::clBlack;                        
                        }
                        
                        bst::rotate_right(parent_node, grandparent_node, &nodes_stack, 1);

                        instead_of_removed_ptr = to_red_black_node(bst::_root_node);
                        break;
                    }
                }

                update_relatives(nodes_stack, &instead_of_removed_ptr, &parent_node, &grandparent_node, &brother_node);
            }

            if (instead_of_removed_ptr != nullptr) {
                to_red_black_node(instead_of_removed_ptr)->color = node_color::clBlack;
            }
            if (bst::_root_node != nullptr) {
                to_red_black_node(bst::_root_node)->color = node_color::clBlack;
            }
        }

        void after_element_removed(
            tkey const & key,
            typename bst::tree_node * element_removed_ptr,
            std::stack<typename bst::tree_node *> & nodes_stack,
            typename bst::tree_node * instead_of_removed_ptr
        ) noexcept override {
            nodes_stack.push(instead_of_removed_ptr);
            if (get_color(to_red_black_node(element_removed_ptr)) == node_color::clBlack) {
                fix_removal(nodes_stack, instead_of_removed_ptr);
            }
        }

        // ===

    };

} // nredblack

#endif //TASK13_RED_BLACK_TREE_H

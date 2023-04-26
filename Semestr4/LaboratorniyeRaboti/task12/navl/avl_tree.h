#ifndef TASK12_AVL_TREE_H
#define TASK12_AVL_TREE_H

#include "../nbst/binary_search_tree.h"
#include "../nbst/associative_container.h"

namespace navl {

    template<
            typename tkey,
            typename tvalue,
            bool (*tkey_comporator)(tkey const &, tkey const &) = nbst::standard_comporators::basic_comporator
            >
    class avl_tree final : public nbst::binary_search_tree<tkey, tvalue, tkey_comporator> {

    protected:

            struct tree_node : public nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::tree_node {

                unsigned int delta;

            };

            unsigned int get_node_size() override {
                return sizeof(tree_node);
            }

            unsigned int get_height(tree_node * node) {
                return node ? node->delta : 0;
            }

            void set_node_height(tree_node * node) {
                unsigned int delta_left = get_height(node->left_child);
                unsigned int delta_right = get_height(node->right_child);
                node->delta = std::max(delta_left, delta_right);
            }

//            void after_left_rotate(
//                    tree_node * rotate_node,
//                    tree_node * saved_right_child
//                    ) override {
//                set_node_height(rotate_node);
//                set_node_height(saved_right_child);
//            }

        tree_node * balance(tree_node * node) {
                if (node->delta == -2) {
                    if (node->right_child->delta > 0) {
                        tree_node * saved_child = node->right_child;
                        node->right_child = rotate_right(node->right_child);
                        set_node_height(saved_child);
                        set_node_height(node->right_child);
                    }
                    tree_node * saved_right_child = rotate_left(node);
                    set_node_height(node);
                    set_node_height(saved_right_child);
                    return saved_right_child;
                }
                if (node->delta == 2) {
                    if (node->delta < 0) {
                        tree_node * saved_child = node->left_child;
                        node->left_child = rotate_left(node->left_child);
                        set_node_height(saved_child);
                        set_node_height(node->left_child);
                    }
                    tree_node * saved_left_child = rotate_right(node);
                    set_node_height(node);
                    set_node_height(saved_left_child);
                    return saved_left_child;
                }
                return node;
            }

//            tree_node * get_avl_node(typename nbst::binary_search_tree<tkey, tvalue, tkey_comporator>::tree_node * node) {
//                return static_cast<avl_tree_node *>(node);
//            }

            void after_element_inserted(
                    std::stack<typename nbst::binary_search_tree<tkey,tvalue, tkey_comporator>::tree_node *> & nodes_stack
                    ) noexcept override {
                tree_node * child_node = nodes_stack.top();
                set_node_height(child_node);
                nodes_stack.pop();
                while (!nodes_stack.empty()) {
                    tree_node * current_node = nodes_stack.top();
                    nodes_stack.pop();
                    if (current_node->left_child == child_node) {
                        ++(current_node->delta);
                    } else {
                        --(current_node->delta);
                    }
                    tree_node * parent_node = nodes_stack.top();
                    if (parent_node->left_child == current_node) {
                        parent_node->left_child = balance(current_node);
                    } else {
                        parent_node->right_child = balance(current_node);
                    }
                    child_node = current_node;
                }
            }

    };

} // navl

#endif //TASK12_AVL_TREE_H

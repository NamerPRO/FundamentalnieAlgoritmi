#ifndef TASK12_AVL_TREE_H
#define TASK12_AVL_TREE_H

#include "../nbst/binary_search_tree.h"
#include "../nbst/associative_container.h"

namespace navl {

    template<typename tkey, typename tvalue, bool (*tkey_comporator)(tkey const &, tkey const &) = nbst::standard_comporators::basic_comporator>
    class avl_tree final : public nbst::binary_search_tree<tkey, tvalue, tkey_comporator> {
    
    public:

        explicit avl_tree(
            nmemory::memory * allocator = nullptr
        ) noexcept : nbst::binary_search_tree<tkey, tvalue, tkey_comporator>(allocator) {}

        avl_tree(
            avl_tree<tkey, tvalue, tkey_comporator> const & tree
        ) noexcept {
            bst::_root_node = nullptr;
            bst::clone(tree);
        }

        avl_tree<tkey, tvalue, tkey_comporator> & operator=(
            avl_tree<tkey, tvalue, tkey_comporator> const & tree
        ) noexcept {
            if (&tree == this) {
                return *this;
            }
            bst::clear();
            bst::clone(tree);
            return *this;
        }

        avl_tree(
            avl_tree<tkey, tvalue, tkey_comporator> && tree
        ) noexcept {
            bst::_root_node = tree._root_node;
            bst::_allocator = tree._allocator;
            tree._root_node = nullptr;
            tree._allocator = nullptr;
        }

        avl_tree<tkey, tvalue, tkey_comporator> & operator=(
            avl_tree<tkey, tvalue, tkey_comporator> && tree
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

        ~avl_tree() noexcept = default;

    protected:

        using bst = nbst::binary_search_tree<tkey, tvalue, tkey_comporator>;

        struct avl_node : public bst::tree_node {

            long long height;

        };

        unsigned int get_node_size() noexcept override {
            return sizeof(avl_node);
        }

        long long get_height(
                typename bst::tree_node * node
                ) noexcept {
            return node ? to_avl_node(node)->height : 0;
        }

        void set_node_height(
                typename bst::tree_node * node
                ) noexcept {
            long long height_left = get_height(node->left_child);
            long long height_right = get_height(node->right_child);
            to_avl_node(node)->height = std::max(height_left, height_right) + 1;
        }

        long long get_balance_faction(
                typename bst::tree_node * node
                ) noexcept {
            return get_height(node->left_child) - get_height(node->right_child);
        }

        void after_left_rotate(
                typename bst::tree_node * rotate_node,
                typename bst::tree_node * saved_right_child
                ) noexcept override {
            set_node_height(rotate_node);
            set_node_height(saved_right_child);
        }

        void after_right_rotate(
                typename bst::tree_node * rotate_node,
                typename bst::tree_node * saved_right_child
        ) noexcept override {
            set_node_height(rotate_node);
            set_node_height(saved_right_child);
        }

        typename bst::tree_node * balance(
                typename bst::tree_node * node
                ) {
            if (get_balance_faction(node) == -2) {
                if (get_balance_faction(node->right_child) > 0) {
                    node->right_child = bst::rotate_right(node->right_child);
                }
                return bst::rotate_left(node);
            }
            if (get_balance_faction(node) == 2) {
                if (get_balance_faction(node->left_child) < 0) {
                    node->left_child = bst::rotate_left(node->left_child);
                }
                return bst::rotate_right(node);
            }
            return node;
        }

        avl_node * to_avl_node(
                typename bst::tree_node * node
                ) noexcept {
            return reinterpret_cast<avl_node *>(node);
        }

        void restore_avl_tree(
                std::stack<typename bst::tree_node *> & nodes_stack
                ) noexcept {
            while (!nodes_stack.empty()) {
                typename bst::tree_node * current_node = nodes_stack.top();
                nodes_stack.pop();
                set_node_height(current_node);

                typename bst::tree_node * subtree_root_node = balance(current_node);
                if (!nodes_stack.empty()) {
                    typename bst::tree_node * parent_node = nodes_stack.top();
                    if (parent_node->left_child == current_node) {
                        parent_node->left_child = subtree_root_node;
                    } else {
                        parent_node->right_child = subtree_root_node;
                    }
                } else {
                    bst::_root_node = subtree_root_node;
                }
            }
        }

        void after_element_inserted(
                    std::stack<typename bst::tree_node *> & nodes_stack
                    ) noexcept override {
            restore_avl_tree(nodes_stack);
        }

        void after_element_removed(
                tkey const & key,
                std::stack<typename bst::tree_node *> & nodes_stack
        ) noexcept override {
            restore_avl_tree(nodes_stack);
        }

    };

} // navl

#endif //TASK12_AVL_TREE_H

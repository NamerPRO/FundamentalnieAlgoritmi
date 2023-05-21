#ifndef SPLAY_TREE
#define SPLAY_TREE

#include "../nbst/binary_search_tree.h"
#include "../nbst/associative_container.h"

#include "../nbst/nmalloc.h"

namespace nsplay {

    template<typename tkey, typename tvalue, bool (*tkey_comporator)(tkey const &, tkey const &) = nbst::standard_comporators::basic_comporator>
    class splay_tree final : public nbst::binary_search_tree<tkey, tvalue, tkey_comporator> {
    
    private:

        using bst = nbst::binary_search_tree<tkey, tvalue, tkey_comporator>;

    public:

        explicit splay_tree(
            nmemory::memory * allocator = nullptr
        ) noexcept : nbst::binary_search_tree<tkey, tvalue, tkey_comporator>(allocator) {}

        splay_tree(
            splay_tree<tkey, tvalue, tkey_comporator> const & tree
        ) noexcept {
            bst::_root_node = nullptr;
            bst::clone(tree);
        }

        splay_tree<tkey, tvalue, tkey_comporator> & operator=(
            splay_tree<tkey, tvalue, tkey_comporator> const & tree
        ) noexcept {
            if (&tree == this) {
                return *this;
            }
            bst::clear();
            bst::clone(tree);
            return *this;
        }

        splay_tree(
            splay_tree<tkey, tvalue, tkey_comporator> && tree
        ) noexcept {
            bst::_root_node = tree._root_node;
            bst::_allocator = tree._allocator;
            tree._root_node = nullptr;
            tree._allocator = nullptr;
        }

        splay_tree<tkey, tvalue, tkey_comporator> & operator=(
            splay_tree<tkey, tvalue, tkey_comporator> && tree
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

        ~splay_tree() noexcept = default;

    private:

        typename std::pair<typename bst::tree_node *, typename bst::tree_node *> get_relatives(
            std::stack<typename bst::tree_node *> nodes_stack
        ) {
            if (nodes_stack.empty()) {
                return { nullptr, nullptr };
            }
            typename bst::tree_node * current_node = nodes_stack.top();
            nodes_stack.pop();
            if (nodes_stack.empty()) {
                nodes_stack.push(current_node);
                return { nullptr, nullptr };
            }
            typename bst::tree_node * parent_node = nodes_stack.top();
            nodes_stack.pop();
            if (nodes_stack.empty()) {
                nodes_stack.push(parent_node);
                nodes_stack.push(current_node);
                return { parent_node, nullptr };
            }
            typename bst::tree_node * grand_parent_node = nodes_stack.top();
            nodes_stack.push(grand_parent_node);
            nodes_stack.push(current_node);
            return { parent_node, grand_parent_node };
        }

        typename bst::tree_node * get_parent(
            std::stack<typename bst::tree_node *> nodes_stack
        ) {
            return get_relatives(nodes_stack).first;
        }

        typename bst::tree_node * get_grand_parent(
            std::stack<typename bst::tree_node *> nodes_stack
        ) {
            return get_relatives(nodes_stack).second;
        }

        void splay(
            std::stack<typename bst::tree_node *> nodes_stack
        ) {
            while (nodes_stack.size() != 1) {
                auto relatives_nodes = get_relatives(nodes_stack);
                typename bst::tree_node * subtree_root_node;
                if (relatives_nodes.second == nullptr) {
                    if (relatives_nodes.first->left_child == nodes_stack.top()) {
                        bst::rotate_right(relatives_nodes.first);
                    } else {
                        bst::rotate_left(relatives_nodes.first);
                    }
                    break;
                } else if (relatives_nodes.second->left_child == relatives_nodes.first
                            && relatives_nodes.first->left_child == nodes_stack.top()) {
                    bst::rotate_right(relatives_nodes.second);
                    subtree_root_node = bst::rotate_right(relatives_nodes.first);
                } else if (relatives_nodes.second->right_child == relatives_nodes.first
                            && relatives_nodes.first->right_child == nodes_stack.top()) {
                    bst::rotate_left(relatives_nodes.second);
                    subtree_root_node = bst::rotate_left(relatives_nodes.first);
                } else if (relatives_nodes.second->left_child == relatives_nodes.first
                            && relatives_nodes.first->right_child == nodes_stack.top()) {
                    relatives_nodes.second->left_child = bst::rotate_left(relatives_nodes.first);
                    subtree_root_node = bst::rotate_right(relatives_nodes.second);
                } else {
                    relatives_nodes.second->right_child = bst::rotate_right(relatives_nodes.first);
                    subtree_root_node = bst::rotate_left(relatives_nodes.second);
                }
                nodes_stack.pop();
                nodes_stack.pop();
                nodes_stack.pop();
                if (!nodes_stack.empty()) {
                   if (nodes_stack.top()->left_child == relatives_nodes.second) {
                        nodes_stack.top()->left_child = subtree_root_node;
                    } else {
                        nodes_stack.top()->right_child = subtree_root_node;
                    } 
                }
                nodes_stack.push(subtree_root_node);
            }
        }

        void after_element_inserted(
            std::stack<typename bst::tree_node *> & nodes_stack
        ) noexcept override {
            splay(nodes_stack);
        }

        void on_insertable_element_exists(
            tkey const & key,
            tvalue && value,
            typename bst::tree_node * found_node,
            std::stack<typename bst::tree_node *> & nodes_stack
        ) noexcept override {
            splay(nodes_stack);
        }

        void after_element_found(
                tkey const & key,
                typename bst::tree_node * found_node,
                std::stack<typename bst::tree_node *> & nodes_stack
        ) noexcept override {
            splay(nodes_stack);
        }

        void merge(
            typename bst::tree_node * left_tree,
            typename bst::tree_node * right_tree
        ) {
            if (left_tree == nullptr && right_tree == nullptr) {
                this->_root_node = nullptr;
                return;
            }
            if (left_tree == nullptr) {
                this->_root_node = right_tree;
                return;
            }
            if (right_tree == nullptr) {
                this->_root_node = left_tree;
                return;
            }
            typename bst::tree_node * current_element = left_tree;
            std::stack<typename bst::tree_node *> nodes_stack;
            nodes_stack.push(current_element);
            while (current_element->right_child != nullptr) {
                current_element = current_element->right_child;
                nodes_stack.push(current_element);
            }
            splay(nodes_stack);
            this->_root_node->right_child = right_tree;
        }

        tvalue remove_element_from_tree(
                std::stack<typename bst::tree_node *> & nodes_stack
        ) override {
            splay(nodes_stack);
            typename bst::tree_node * element_to_remove_ptr = nodes_stack.top();
            tvalue element_to_remove_data = std::move(element_to_remove_ptr->value);
            merge(element_to_remove_ptr->left_child, element_to_remove_ptr->right_child);
            element_to_remove_ptr->~tree_node();
            ntools::nmalloc::free(element_to_remove_ptr);
            return element_to_remove_data;
        }

    };

}

#endif /* SPLAY_TREE */

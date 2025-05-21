#pragma once

#include <memory>
#include <functional>
#include <iterator>
#include <utility>
#include <cstddef>

namespace my_bst {

    template <typename T, typename Allocator>
    struct TreeNode {
        T value;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        TreeNode* parent = nullptr;

        explicit TreeNode(const T& val) : value(val) {}
    };

    template <typename NodePtr, typename T>
    class BSTBaseIterator {
    protected:
        NodePtr current;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;

        explicit BSTBaseIterator(NodePtr node = nullptr) : current(node) {}

        reference operator*() const { return current->value; }
        pointer operator->() const { return &current->value; }

        bool operator==(const BSTBaseIterator& other) const {
            return current == other.current;
        }
        bool operator!=(const BSTBaseIterator& other) const {
            return !(*this == other);
        }

    protected:
        NodePtr get_node() const { return current; }
    };

    template <typename NodePtr, typename T>
    class InOrderIterator : public BSTBaseIterator<NodePtr, T> {
        using Base = BSTBaseIterator<NodePtr, T>;
        NodePtr root;

    public:
        InOrderIterator(NodePtr root = nullptr, NodePtr node = nullptr)
            : Base(node), root(root) {}

        InOrderIterator& operator++() {
            if (!this->current) {
                this->current = root;
                while (this->current && this->current->left)
                    this->current = this->current->left;
                return *this;
            }

            if (this->current->right) {
                this->current = this->current->right;
                while (this->current->left)
                    this->current = this->current->left;
            }
            else {
                NodePtr p = this->current->parent;
                while (p && this->current == p->right) {
                    this->current = p;
                    p = p->parent;
                }
                this->current = p;
            }
            return *this;
        }

        InOrderIterator operator++(int) {
            InOrderIterator tmp = *this;
            ++*this;
            return tmp;
        }

        InOrderIterator& operator--() {
            if (!this->current) {
                this->current = root;
                while (this->current && this->current->right)
                    this->current = this->current->right;
                return *this;
            }

            if (this->current->left) {
                this->current = this->current->left;
                while (this->current->right)
                    this->current = this->current->right;
            }
            else {
                NodePtr p = this->current->parent;
                while (p && this->current == p->left) {
                    this->current = p;
                    p = p->parent;
                }
                this->current = p;
            }
            return *this;
        }

        InOrderIterator operator--(int) {
            InOrderIterator tmp = *this;
            --*this;
            return tmp;
        }
    };

    template <typename NodePtr, typename T>
    class PreOrderIterator : public BSTBaseIterator<NodePtr, T> {
        using Base = BSTBaseIterator<NodePtr, T>;

    public:
        explicit PreOrderIterator(NodePtr node = nullptr) : Base(node) {}

        PreOrderIterator& operator++() {
            if (!this->current) return *this;

            if (this->current->left) {
                this->current = this->current->left;
            }
            else if (this->current->right) {
                this->current = this->current->right;
            }
            else {
                NodePtr p = this->current->parent;
                while (p) {
                    if (this->current == p->left && p->right) {
                        this->current = p->right;
                        return *this;
                    }
                    this->current = p;
                    p = p->parent;
                }
                this->current = nullptr;
            }
            return *this;
        }

        PreOrderIterator operator++(int) {
            PreOrderIterator tmp = *this;
            ++*this;
            return tmp;
        }
    };

    template <typename NodePtr, typename T>
    class PostOrderIterator : public BSTBaseIterator<NodePtr, T> {
        using Base = BSTBaseIterator<NodePtr, T>;

    public:
        explicit PostOrderIterator(NodePtr node = nullptr) : Base(node) {}

        PostOrderIterator& operator++() {
            if (!this->current) return *this;

            NodePtr p = this->current->parent;
            if (!p) {
                this->current = nullptr;
                return *this;
            }

            if (this->current == p->left && p->right) {
                this->current = p->right;
                while (true) {
                    if (this->current->left) {
                        this->current = this->current->left;
                    }
                    else if (this->current->right) {
                        this->current = this->current->right;
                    }
                    else {
                        break;
                    }
                }
            }
            else {
                this->current = p;
            }
            return *this;
        }

        PostOrderIterator operator++(int) {
            PostOrderIterator tmp = *this;
            ++*this;
            return tmp;
        }
    };

    template <typename T,
        typename Compare = std::less<T>,
        typename Allocator = std::allocator<T>>
        class BinarySearchTree {
        public:
            using value_type = T;
            using key_type = T;
            using key_compare = Compare;
            using allocator_type = Allocator;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using reference = T&;
            using const_reference = const T&;

            using Node = TreeNode<T, Allocator>;
            using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

            using inorder_iterator = InOrderIterator<Node*, T>;
            using const_inorder_iterator = InOrderIterator<const Node*, const T>;
            using preorder_iterator = PreOrderIterator<Node*, T>;
            using const_preorder_iterator = PreOrderIterator<const Node*, const T>;
            using postorder_iterator = PostOrderIterator<Node*, T>;
            using const_postorder_iterator = PostOrderIterator<const Node*, const T>;
            using reverse_iterator = std::reverse_iterator<inorder_iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_inorder_iterator>;

            BinarySearchTree(const Compare& comp = Compare(),
                const Allocator& alloc = Allocator())
                : root_(nullptr), node_count_(0), comp_(comp), alloc_(alloc) {}

            BinarySearchTree(const BinarySearchTree& other)
                : comp_(other.comp_), alloc_(other.alloc_) {
                root_ = clone_tree(other.root_, nullptr);
                node_count_ = other.node_count_;
            }

            BinarySearchTree(BinarySearchTree&& other) noexcept
                : root_(other.root_), node_count_(other.node_count_),
                comp_(std::move(other.comp_)), alloc_(std::move(other.alloc_)) {
                other.root_ = nullptr;
                other.node_count_ = 0;
            }

            ~BinarySearchTree() { clear(); }

            BinarySearchTree& operator=(const BinarySearchTree& other) {
                if (this != &other) {
                    clear();
                    comp_ = other.comp_;
                    alloc_ = other.alloc_;
                    root_ = clone_tree(other.root_, nullptr);
                    node_count_ = other.node_count_;
                }
                return *this;
            }

            BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
                if (this != &other) {
                    clear();
                    root_ = other.root_;
                    node_count_ = other.node_count_;
                    comp_ = std::move(other.comp_);
                    alloc_ = std::move(other.alloc_);
                    other.root_ = nullptr;
                    other.node_count_ = 0;
                }
                return *this;
            }

            bool empty() const noexcept { return node_count_ == 0; }
            size_type size() const noexcept { return node_count_; }

            bool contains(const T& value) const { return find_node(value) != nullptr; }

            std::pair<inorder_iterator, bool> insert(const T& value) {
                Node* parent = nullptr;
                auto curr = root_;

                while (curr) {
                    parent = curr;
                    if (comp_(value, curr->value)) {
                        curr = curr->left;
                    }
                    else if (comp_(curr->value, value)) {
                        curr = curr->right;
                    }
                    else {
                        return { inorder_iterator(root_, curr), false };
                    }
                }

                Node* new_node = create_node(value);
                new_node->parent = parent;

                if (!parent) {
                    root_ = new_node;
                }
                else if (comp_(value, parent->value)) {
                    parent->left = new_node;
                }
                else {
                    parent->right = new_node;
                }

                ++node_count_;
                return { inorder_iterator(root_, new_node), true };
            }

            size_type erase(const T& key) {
                auto node = find_node(key);
                if (!node) return 0;

                remove_node(node);
                --node_count_;
                return 1;
            }

            void clear() noexcept {
                destroy_subtree(root_);
                root_ = nullptr;
                node_count_ = 0;
            }

            inorder_iterator begin() noexcept {
                Node* curr = root_;
                while (curr && curr->left) curr = curr->left;
                return inorder_iterator(root_, curr);
            }

            inorder_iterator end() noexcept {
                return inorder_iterator(root_, nullptr);
            }

            const_inorder_iterator begin() const noexcept {
                return const_cast<BinarySearchTree*>(this)->begin();
            }

            const_inorder_iterator end() const noexcept {
                return const_cast<BinarySearchTree*>(this)->end();
            }

            preorder_iterator pre_begin() noexcept {
                return preorder_iterator(root_);
            }

            preorder_iterator pre_end() noexcept {
                return preorder_iterator(nullptr);
            }

            const_preorder_iterator pre_begin() const noexcept {
                return const_cast<BinarySearchTree*>(this)->pre_begin();
            }

            const_preorder_iterator pre_end() const noexcept {
                return const_cast<BinarySearchTree*>(this)->pre_end();
            }

            postorder_iterator post_begin() noexcept {
                Node* curr = root_;
                while (curr && (curr->left || curr->right)) {
                    if (curr->left) curr = curr->left;
                    else curr = curr->right;
                }
                return postorder_iterator(curr);
            }

            postorder_iterator post_end() noexcept {
                return postorder_iterator(nullptr);
            }

            const_postorder_iterator post_begin() const noexcept {
                return const_cast<BinarySearchTree*>(this)->post_begin();
            }

            const_postorder_iterator post_end() const noexcept {
                return const_cast<BinarySearchTree*>(this)->post_end();
            }

            inorder_iterator find(const T& key) {
                return inorder_iterator(root_, find_node(key));
            }

            const_inorder_iterator find(const T& key) const {
                return const_inorder_iterator(root_, find_node(key));
            }

            reverse_iterator rbegin() noexcept {
                return reverse_iterator(end());
            }

            reverse_iterator rend() noexcept {
                return reverse_iterator(begin());
            }

            const_reverse_iterator rbegin() const noexcept {
                return const_reverse_iterator(end());
            }

            const_reverse_iterator rend() const noexcept {
                return const_reverse_iterator(begin());
            }

        private:
            Node* root_ = nullptr;
            size_type node_count_ = 0;
            Compare comp_;
            NodeAllocator alloc_;

            Node* create_node(const T& value) {
                Node* node = alloc_.allocate(1);
                alloc_.construct(node, value);
                return node;
            }

            void destroy_node(Node* node) noexcept {
                if (node) {
                    alloc_.destroy(node);
                    alloc_.deallocate(node, 1);
                }
            }

            Node* clone_tree(Node* node, Node* parent) {
                if (!node) return nullptr;

                Node* new_node = create_node(node->value);
                new_node->parent = parent;
                new_node->left = clone_tree(node->left, new_node);
                new_node->right = clone_tree(node->right, new_node);

                return new_node;
            }

            void destroy_subtree(Node* node) noexcept {
                if (!node) return;
                destroy_subtree(node->left);
                destroy_subtree(node->right);
                destroy_node(node);
            }

            Node* find_node(const T& key) const {
                auto curr = root_;
                while (curr) {
                    if (comp_(key, curr->value)) {
                        curr = curr->left;
                    }
                    else if (comp_(curr->value, key)) {
                        curr = curr->right;
                    }
                    else {
                        return curr;
                    }
                }
                return nullptr;
            }

            void transplant(Node* u, Node* v) {
                if (!u->parent) {
                    root_ = v;
                }
                else if (u == u->parent->left) {
                    u->parent->left = v;
                }
                else {
                    u->parent->right = v;
                }

                if (v) v->parent = u->parent;
            }

            void remove_node(Node* z) {
                if (!z->left) {
                    transplant(z, z->right);
                }
                else if (!z->right) {
                    transplant(z, z->left);
                }
                else {
                    auto y = z->right;
                    while (y->left) y = y->left;

                    if (y->parent != z) {
                        transplant(y, y->right);
                        y->right = z->right;
                        y->right->parent = y;
                    }

                    transplant(z, y);
                    y->left = z->left;
                    y->left->parent = y;
                }

                destroy_node(z);
            }
    };

}
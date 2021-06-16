#pragma once

#include <list>
#include <memory>
#include <type_traits>

namespace task {
template <typename T, typename Allocator = std::allocator<T>>
class List {
private:
    struct Node {
        Node* prev;
        Node* next;
        T value;

        Node(Node* prev, Node* next, const T& value) : prev(prev), next(next), value(value) {
        }
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;

public:
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator() = delete;
        explicit Iterator(Node* node, bool end = false) : node_(node), end_(end) {
        }

        Iterator& operator++() {
            if (node_->next == nullptr) {
                end_ = true;
            } else {
                node_ = node_->next;
            }
            return *this;
        }

        Iterator& operator--() {
            if (!end_) {
                node_ = node_->prev;
            }
            end_ = false;
            return *this;
        }

        Iterator operator++(int) {
            Iterator it(*this);
            operator++();
            return it;
        }

        Iterator operator--(int) {
            Iterator it(*this);
            operator--();
            return it;
        }

        pointer operator->() const {
            if (end_) {
                throw std::runtime_error("End iterator");
            }
            return &node_->value;
        }

        reference operator*() const {
            if (end_) {
                throw std::runtime_error("End iterator");
            }
            return node_->value;
        }

        bool operator==(const Iterator& other) {
            return node_ == other.node_ && end_ == other.end_;
        }

        bool operator!=(const Iterator& other) {
            return !(*this == other);
        }

    private:
        Node* node_;
        bool end_;
    };

public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = Iterator;
    using const_iterator = const Iterator;
    using allocator_type = Allocator;
    using propagate_on_container_swap = std::true_type;

    typedef typename std::allocator_traits<Allocator>::template rebind_alloc<Node> node_allocator;


    // List variables
    node_allocator alloc_;
    // Your code goes here

    // Special member functions
    List(){};

    List(const List& other) {
        size_ = 0;
        auto tmp = other.head_;
        while (tmp!= other.tail_) {
            PushBack(tmp->value);
            tmp = tmp->next;
        }
        PushBack(tmp->value);
    }
    List(const List& other, const Allocator& alloc) {
        size_ = 0;
        auto tmp = other.head_;
        while (tmp!= other.tail_) {
            PushBack(tmp->value);
            tmp = tmp->next;
        }
        PushBack(tmp->value);
    }

    List(List&& other) {
        *this = std::move(other);
    }
    List(List&& other, const Allocator& alloc) {
        alloc_ = alloc;
        auto tmp = other.head_;
        while (tmp!= other.tail_) {
            PushBack(std::move(tmp->value));
            tmp = tmp->next;
        }
        PushBack(std::move(tmp->value));
        size_ = other.Size();
        other.Clear();
    }

    ~List() {
        Clear();
    }

    List& operator=(const List& other) {
        Clear();
        auto tmp = other;
        while (tmp.head_ != tmp.tail_) {
            PushBack(tmp.head_->value);
            tmp.head_ = tmp.head_->next;
        }
        PushBack(tmp.head_->value);
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        Clear();
        if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
            alloc_ = other.alloc_;

            head_ = other.head_;
            tail_ = other.tail_;

            other.head_ = nullptr;
            other.tail_ = nullptr;
            size_ = other.size_;
        } else {
            for (Node* node = other.head_; node != nullptr; node = node->next) {
                PushBack(std::move(node->value));
            }
            other.Clear();
        }

        return *this;
    }

    // Element access
    reference Front() {
        return head_->value;
    }
    const_reference Front() const {
        return head_->value;
    }
    reference Back() {
        return tail_->value;
    }
    const_reference Back() const {
        return tail_->value;
    }

    // Iterators
    iterator Begin() noexcept {
        return Iterator(head_, head_ == nullptr);
    }
    const_iterator Begin() const noexcept {
        return Iterator(head_, head_ == nullptr);
    }

    iterator End() noexcept {
        return Iterator(tail_, true);
    }
    const_iterator End() const noexcept {
        return Iterator(tail_, true);
    }

    // Capacity
    bool Empty() const noexcept {
        return size_ == 0;
    }

    size_type Size() const noexcept {
        return size_;
    }
    size_type MaxSize() const noexcept {
        return size_;
    }

    // Modifiers
    void Clear() {
        while (size_ != 0) {
            PopBack();
        }
        size_ = 0;
    }
    void Swap(List& other) noexcept {
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(size_, other.size_);
            std::swap(head_, other.head_);
            std::swap(tail_, other.tail_);
            std::swap(alloc_, other.alloc_);
        } else {
            List temp = std::move(other);
            other = std::move(*this);
            *this = std::move(temp);
        }
    }

    void PushBack(const T& value) {
        Node* node = alloc_.allocate(1);
        alloc_.construct(node, tail_, nullptr, value);

        if (tail_ != nullptr) {
            tail_->next = node;
        }
        tail_ = node;
        if (head_ == nullptr) {
            head_ = node;
        }
        size_ += 1;
    }
    void PushBack(T&& value) {
        Node* node = alloc_.allocate(1);
        alloc_.construct(node, tail_, nullptr, std::move(value));

        if (tail_ != nullptr) {
            tail_->next = node;
        }
        tail_ = node;
        if (head_ == nullptr) {
            head_ = node;
        }
        size_ += 1;
    }
    
    template <typename... Args>
    void EmplaceBack(Args&&... args) {
        Node* e = alloc_.allocate(1);
        alloc_.construct(e, tail_, nullptr, std::forward<Args>(args)...);

        if (tail_ != nullptr) {
            tail_->next = e;
        }
        tail_ = e;
        if (head_ == nullptr) {
            head_ = e;
        }
        size_ += 1;
    }
    void PopBack() {
        if (tail_ == nullptr) {
            size_ = 0;
            return;
        }
        Node* node = tail_;
        tail_ = node->prev;
        if (tail_ != nullptr) {
            tail_->next = nullptr;
        } else {
            head_ = nullptr;
        }

        alloc_.destroy(node);
        alloc_.deallocate(node, 1);
        size_ -= 1;
    }
    void PushFront(const T& value) {
        Node* e = alloc_.allocate(1);
        alloc_.construct(e, nullptr, head_, value);

        if (head_ != nullptr) {
            head_->prev = e;
        }
        head_ = e;
        if (tail_ == nullptr) {
            tail_ = e;
        }
        size_ += 1;
    }
    void PushFront(T&& value) {
        Node* e = alloc_.allocate(1);
        alloc_.construct(e, nullptr, head_, value);

        if (head_ != nullptr) {
            head_->prev = e;
        }
        head_ = e;
        if (tail_ == nullptr) {
            tail_ = e;
        }
    }
    template <typename... Args>
    void EmplaceFront(Args&&... args) {
        Node* node = alloc_.allocate(1);
        alloc_.construct(node, nullptr, head_, std::forward<Args>(args)...);

        if (head_ != nullptr) {
            head_->prev = node;
        }
        head_ = node;
        if (tail_ == nullptr) {
            tail_ = node;
        }
    }
    void PopFront() {
        if (head_ == nullptr) {
            return;
        }

        Node* node = head_;
        head_ = node->next;
        if (head_ != nullptr) {
            head_->prev = nullptr;
        } else {
            tail_ = nullptr;
        }

        alloc_.destroy(node);
        alloc_.deallocate(node, 1);
    }

    void Resize(size_type count) {
        size_type size = Size();
        if (size < count) {
            for (size_type i = 0; i < count - size; ++i) {
                PushBack(T());
            }
        } else {
            for (size_type i = 0; i < size - count; ++i) {
                PopBack();
            }
        }
    }

    // Operations
    void Remove(const T& value) {
        Node* node = head_;

        while (node != nullptr) {
            Node* next;

            if (node->value != value) {
                next = node->next;
            } else {
                if (node->prev != nullptr) {
                    node->prev->next = node->next;
                }
                if (node->next != nullptr) {
                    node->next->prev = node->prev;
                }
                if (node == head_) {
                    head_ = node->next;
                }
                if (node == tail_) {
                    tail_ = node->prev;
                }

                next = node->next;

                alloc_.destroy(node);
                alloc_.deallocate(node, 1);
            }

            node = next;
        }
    }
    void Unique() {
        Node* node = head_;
        std::set<T> els;

        while (node != nullptr) {
            Node* next;

            if (els.find(node->value) == els.end()) {
                els.insert(node->value);
                next = node->next;
            } else {
                node->prev->next = node->next;
                if (node->next != nullptr) {
                    node->next->prev = node->prev;
                }
                if (node == head_) {
                    head_ = node->next;
                }
                if (node == tail_) {
                    tail_ = node->prev;
                }

                next = node->next;

                alloc_.destroy(node);
                alloc_.deallocate(node, 1);
            }

            node = next;
        }
    }
    void Sort() {
        size_type size = Size();
        for (size_type i = 0; i < size; ++i) {
            Node* node = head_;

            for (size_type j = 0; j < size - i - 1; ++j, node = node->next) {
                if (node->value > node->next->value) {
                    std::swap(node->value, node->next->value);
                }
            }
        }
    }

    allocator_type GetAllocator() const noexcept {
        return Allocator(alloc_);
    }

private:
    size_t size_ = 0;
    // Your code goes here
};

}  // namespace task
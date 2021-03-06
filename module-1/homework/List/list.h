#pragma once
#include <cstddef>


namespace task {

    struct ListNode {
        int val;
        ListNode* next = nullptr;
        ListNode* prev = nullptr;
        ListNode(int elem): val(elem) {}
    };

class list {
public:
    list();
    list(size_t count, const int& value = int());
    list(const list& tmp);
    list(list&& tmp);

    ~list();
    list& operator=(const list& other);
    list& operator=(list&& other);


    int& front();
    const int& front() const;

    int& back();
    const int& back() const;


    bool empty() const;
    size_t size() const;
    void clear();


    void push_back(const int& value);
    void pop_back();
    void push_front(const int& value);
    void pop_front();
    void resize(size_t count);
    void swap(list& other);


    void remove(const int value);
    void unique();
    void sort();

    // Your code goes here?..

private:
    size_t size_;
    ListNode* head_ = nullptr;
    ListNode* tail_ = nullptr;

    void remove_node_(ListNode* node);

};

}  // namespace task

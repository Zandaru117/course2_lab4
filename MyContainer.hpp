#pragma once

#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class MyContainer {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v);
    };

    using NodeAlloc =
        typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;

    NodeAlloc alloc_;
    Node* head_;
    Node* tail_;

public:
    MyContainer(const Alloc& a = Alloc());
    ~MyContainer();

    void push_back(const T& v);

    struct iterator {
        Node* p;
        iterator(Node* x);
        T& operator*();
        iterator& operator++();
        bool operator!=(const iterator& other) const;
    };

    iterator begin();
    iterator end();
};

#include "MyContainer.tpp"

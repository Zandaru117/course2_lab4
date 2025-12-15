#pragma once

#include <cstddef>

template <typename T, std::size_t N>
class MyAllocator {
private:
    struct Node {
        Node* next;
    };

    struct Pool {
        char* memory;
        Node* free_list;
        Pool* next;
    };

    Pool* pools_;

    Pool* create_pool();
    bool owns(T* p, Pool* pool) const;

public:
    using value_type = T;

    template <class U>
    struct rebind {
        using other = MyAllocator<U, N>;
    };

    MyAllocator() noexcept;
    ~MyAllocator();

    template <class U>
    MyAllocator(const MyAllocator<U, N>&) noexcept;

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);

    bool operator==(const MyAllocator&) const noexcept { return true; }
    bool operator!=(const MyAllocator&) const noexcept { return false; }
};

#include "MyAllocator.tpp"
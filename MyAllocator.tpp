#include <new>

template <typename T, std::size_t N>
MyAllocator<T, N>::MyAllocator() noexcept
    : pools_(nullptr) {
    pools_ = create_pool();
}

template <typename T, std::size_t N>
template <class U>
MyAllocator<T, N>::MyAllocator(const MyAllocator<U, N>&) noexcept
    : pools_(nullptr) {
    pools_ = create_pool();
}

template <typename T, std::size_t N>
MyAllocator<T, N>::~MyAllocator() {
    while (pools_) {
        ::operator delete(pools_->memory);
        Pool* next = pools_->next;
        delete pools_;
        pools_ = next;
    }
}

template <typename T, std::size_t N>
typename MyAllocator<T, N>::Pool*
MyAllocator<T, N>::create_pool() {
    Pool* pool = new Pool;
    pool->memory = static_cast<char*>(::operator new(sizeof(T) * N));
    pool->free_list = nullptr;
    pool->next = pools_;

    for (std::size_t i = 0; i < N; ++i) {
        Node* node =
            reinterpret_cast<Node*>(pool->memory + i * sizeof(T));
        node->next = pool->free_list;
        pool->free_list = node;
    }
    return pool;
}

template <typename T, std::size_t N>
bool MyAllocator<T, N>::owns(T* p, Pool* pool) const {
    char* c = reinterpret_cast<char*>(p);
    return c >= pool->memory &&
           c < pool->memory + sizeof(T) * N;
}

template <typename T, std::size_t N>
T* MyAllocator<T, N>::allocate(std::size_t n) {
    if (n != 1)
        return static_cast<T*>(::operator new(n * sizeof(T)));

    Pool* pool = pools_;
    while (pool) {
        if (pool->free_list) {
            Node* node = pool->free_list;
            pool->free_list = node->next;
            return reinterpret_cast<T*>(node);
        }
        pool = pool->next;
    }

    pools_ = create_pool();
    Node* node = pools_->free_list;
    pools_->free_list = node->next;
    return reinterpret_cast<T*>(node);
}

template <typename T, std::size_t N>
void MyAllocator<T, N>::deallocate(T* p, std::size_t n) {
    if (!p) return;

    if (n != 1) {
        ::operator delete(p);
        return;
    }

    Pool* pool = pools_;
    while (pool) {
        if (owns(p, pool)) {
            Node* node = reinterpret_cast<Node*>(p);
            node->next = pool->free_list;
            pool->free_list = node;
            return;
        }
        pool = pool->next;
    }
}
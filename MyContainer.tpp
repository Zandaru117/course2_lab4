template <typename T, typename Alloc>
MyContainer<T, Alloc>::Node::Node(const T& v)
    : value(v), next(nullptr) {}

template <typename T, typename Alloc>
MyContainer<T, Alloc>::MyContainer(const Alloc& a)
    : alloc_(a), head_(nullptr), tail_(nullptr) {}

template <typename T, typename Alloc>
MyContainer<T, Alloc>::~MyContainer() {
    while (head_) {
        Node* tmp = head_;
        head_ = head_->next;
        std::allocator_traits<NodeAlloc>::destroy(alloc_, tmp);
        alloc_.deallocate(tmp, 1);
    }
}

template <typename T, typename Alloc>
void MyContainer<T, Alloc>::push_back(const T& v) {
    Node* n = alloc_.allocate(1);
    std::allocator_traits<NodeAlloc>::construct(alloc_, n, v);

    if (!head_) head_ = tail_ = n;
    else {
        tail_->next = n;
        tail_ = n;
    }
}

template <typename T, typename Alloc>
MyContainer<T, Alloc>::iterator::iterator(Node* x) : p(x) {}

template <typename T, typename Alloc>
T& MyContainer<T, Alloc>::iterator::operator*() {
    return p->value;
}

template <typename T, typename Alloc>
typename MyContainer<T, Alloc>::iterator&
MyContainer<T, Alloc>::iterator::operator++() {
    p = p->next;
    return *this;
}

template <typename T, typename Alloc>
bool MyContainer<T, Alloc>::iterator::operator!=(const iterator& other) const {
    return p != other.p;
}

template <typename T, typename Alloc>
typename MyContainer<T, Alloc>::iterator
MyContainer<T, Alloc>::begin() {
    return iterator(head_);
}

template <typename T, typename Alloc>
typename MyContainer<T, Alloc>::iterator
MyContainer<T, Alloc>::end() {
    return iterator(nullptr);
}
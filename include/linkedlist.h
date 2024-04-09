#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdexcept>
#include <cassert>

template<typename T>
class LinkedList {
protected:
    size_t length;
    struct Node {
        T value;
        Node *next = nullptr;
    } *first, *last;

    struct {
        size_t idx;
        Node *node;
    } cache;

    Node* get_node_unamortized(size_t idx) const;
    Node* _get_node(size_t idx);
    Node* get_node(size_t idx) const;

private:
    void destroy_nodes();

public:
    using value_type = T;

    LinkedList();
    LinkedList(const LinkedList& src);
    LinkedList(LinkedList&& src) noexcept;

    ~LinkedList();

    [[nodiscard]]
    size_t size() const noexcept;
    [[nodiscard]]
    bool empty() const noexcept;

    void push_back(const T& element);
    void push_front(const T& element);

    void insert(size_t pos, const T& element);
    void remove(size_t idx);

    void clear();

    T& front();
    T& back();

    T& at(size_t idx);
    const T& at(size_t idx) const;

    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;

    bool operator==(const LinkedList& other) const noexcept;
    bool operator!=(const LinkedList& other) const noexcept;

    LinkedList& operator=(const LinkedList& other);
    LinkedList& operator=(LinkedList&& other) noexcept;

    template<typename U>
    class Iterator
    {
        friend class LinkedList;
    private:
        Node* cur;

        Iterator(Node* node);
    public:
        using value_type = T;

        Iterator& operator++();

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

        U& operator*() const;
        U* operator->() const;
    };

    using iterator               = Iterator<T>;
    using const_iterator         = Iterator<const T>;

    iterator begin();
    iterator end();
    //
    const_iterator begin() const;
    const_iterator end() const;
    //
    const_iterator cbegin() const;
    const_iterator cend() const;

    friend void swap(LinkedList& lhs, LinkedList& rhs) noexcept
    {
        std::swap(lhs.length, rhs.length);
        std::swap(lhs.first, rhs.first);
        std::swap(lhs.last, rhs.last);
        std::swap(lhs.cache, rhs.cache);
    }
};

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::get_node_unamortized(size_t idx) const
{
    Node *node = first;
    for (size_t i = 0; i < idx; i++)
        node = node->next;
    return node;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::_get_node(size_t idx)
{
    size_t i;
    Node *node;

    if (cache.idx <= idx)
    {
        i = cache.idx;
        node = cache.node;
    } else {
        i = 0;
        node = first;
    }

    for ( ; i < idx; i++) {
        node = node->next;
    }

    cache.idx = idx;
    cache.node = node;

    return node;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::get_node(size_t idx) const
{
    return const_cast<LinkedList*>(this)->_get_node(idx);
}

template<typename T>
void LinkedList<T>::destroy_nodes()
{
    Node *current = first, *garbage;
    while (current)
    {
        garbage = current;
        current = garbage->next;
        delete garbage;
    }
}

template<typename T>
LinkedList<T>::LinkedList()
        : length(0)
        , first(nullptr)
        , last(nullptr)
        , cache({ 0, nullptr })
{}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& src)
        : length(src.length)
{
    if (!src.first) {
        first = last = nullptr;
        return;
    }

    Node *cur = first = new Node{ *src.first };
    for (; cur->next; cur = cur->next) {
        cur->next = new Node{ *cur->next };
    }
    last = cur;

    cache = { 0, first };
}

template<typename T>
LinkedList<T>::LinkedList(LinkedList&& src) noexcept
        : first(nullptr)
        , last(nullptr)
{
    swap(*this, src);
}

template<typename T>
LinkedList<T>::~LinkedList()
{
    destroy_nodes();
}

template<typename T>
size_t LinkedList<T>::size() const noexcept
{
    return length;
}

template<typename T>
bool LinkedList<T>::empty() const noexcept
{
    return length == 0;
}

template<typename T>
void LinkedList<T>::push_back(const T& element)
{
    auto node = new Node { element };

    if (first == nullptr)
    {
        first = last = node;
        cache.node = first;
    }
    else
    {
        last->next = node;
        last = node;
    }

    length++;
}

template<typename T>
void LinkedList<T>::push_front(const T& element)
{
    first = new Node { element, first };
    length++;

    cache.idx++;
}

template<typename T>
void LinkedList<T>::insert(size_t pos, const T& element)
{
    assert(pos < length && "Index is out of range");

    if (pos == 0) {
        push_front(element);
        return;
    } else if (pos == length - 1) {
        last->next = new Node { element, nullptr };
        last = last->next;
        length++;
        return;
    }

    Node *node = get_node(pos - 1);
    node->next = new Node { element, node->next };
    length++;

    if (pos <= cache.idx) { // should be always false because of get_node(pos - 1) call
        cache.idx++;
    }
}

template<typename T>
void LinkedList<T>::remove(size_t idx)
{
    assert(idx < length && "Index is out of range");

    Node* node;
    if (idx == 0)
    {
        node = first;
        first = node->next;
    }
    else
    {
        Node* prev = get_node(idx - 1);
        node = prev->next;
        prev->next = node->next;
    }

    length--;

    if (idx == cache.idx)
    {
        cache.node = cache.node->next;
        if (!cache.node) {
            cache.node = first;
            cache.idx = 0;
        }
    }
    else if (idx < cache.idx)
    {
        cache.idx--;
    }

    delete node;
}

template<typename T>
T& LinkedList<T>::front()
{
    assert(length && "List is empty");
    return first->value;
}

template<typename T>
T& LinkedList<T>::back()
{
    assert(length && "List is empty");
    return last->value;
}

template<typename T>
T& LinkedList<T>::at(const size_t idx)
{
    return const_cast<T&>(std::as_const(*this).at(idx));
}

template<typename T>
const T& LinkedList<T>::at(const size_t idx) const
{
    return (*this)[idx];
}

template<typename T>
void LinkedList<T>::clear()
{
    destroy_nodes();
    first = last = nullptr;
    length = 0;
}

template<typename T>
T& LinkedList<T>::operator[](const size_t idx)
{
    return const_cast<T&>(std::as_const(*this)[idx]);
}

template<typename T>
const T& LinkedList<T>::operator[](const size_t idx) const
{
    assert(idx < length && "Index is out of range"); // stripped out from release
    return get_node(idx)->value;
}

template<typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& other) const noexcept
{
    if (this == &other)
        return true;

    if (length != other.length)
        return false;

    Node *left = first, *right = other.first;
    while (left)
    {
        if (left->value != right->value)
            return false;

        left = left->next;
        right = right->next;
    }

    return true;
}

template<typename T>
bool LinkedList<T>::operator!=(const LinkedList<T>& other) const noexcept
{
    return !(*this == other);
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
    if (this == &other)
        return *this;

    LinkedList<T> tmp(other);
    swap(*this, tmp);
    return *this;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList &&other) noexcept
{
    swap(*this, other);
    return *this;
}

// region Iterator Support

template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin()
{
    return first;
}

template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::end()
{
    return nullptr;
}

template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::begin() const
{
    return first;
}

template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::end() const
{
    return nullptr;
}

template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cbegin() const
{
    return first;
}

template<typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cend() const
{
    return nullptr;
}

// endregion

// region Iterator

template<typename T>
template<typename U>
LinkedList<T>::Iterator<U>::Iterator(LinkedList::Node *node)
    : cur(node)
{}

template<typename T>
template<typename U>
typename LinkedList<T>::template Iterator<U>& LinkedList<T>::Iterator<U>::operator++()
{
    cur = cur->next;
    return *this;
}

template<typename T>
template<typename U>
bool LinkedList<T>::Iterator<U>::operator==(const typename LinkedList<T>::template Iterator<U> &other) const
{
    return cur == other.cur;
}

template<typename T>
template<typename U>
bool LinkedList<T>::Iterator<U>::operator!=(const typename LinkedList<T>::template Iterator<U> &other) const
{
    return !(*this == other);
}

template<typename T>
template<typename U>
U& LinkedList<T>::Iterator<U>::operator*() const
{
    return cur->value;
}

template<typename T>
template<typename U>
U* LinkedList<T>::Iterator<U>::operator->() const
{
    return &cur->value;
}

// endregion

#endif // __LINKEDLIST_H__

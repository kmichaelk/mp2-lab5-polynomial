#ifndef __ORDERED_LINKEDLIST_H__
#define __ORDERED_LINKEDLIST_H__

#include <functional>

#include "linkedlist.h"

template<typename T>
class OrderedLinkedList : private LinkedList<T>
{
protected:
    using typename LinkedList<T>::Node;
public:
    // -- yields true if the first argument of the call appears before the second and false otherwise
    typedef std::function<bool(const T& a, const T& b)> CompareFunction;
private:
    CompareFunction comp;
public:

    explicit OrderedLinkedList(const CompareFunction& comp = [](const T& a, const T& b) { return a < b; });

    void push(const T& element);

    using LinkedList<T>::size;
    using LinkedList<T>::empty;

    using LinkedList<T>::remove;
    using LinkedList<T>::clear;

    using LinkedList<T>::front;
    using LinkedList<T>::back;

    using LinkedList<T>::at;

    using LinkedList<T>::operator[];

    using LinkedList<T>::operator=;

    using LinkedList<T>::begin;
    using LinkedList<T>::end;
    using LinkedList<T>::cbegin;
    using LinkedList<T>::cend;

    bool operator==(const OrderedLinkedList& other) const;
    bool operator!=(const OrderedLinkedList& other) const;

    friend void swap(OrderedLinkedList& lhs, OrderedLinkedList& rhs) noexcept
    {
        swap(static_cast<LinkedList<T>&>(lhs), static_cast<LinkedList<T>&>(rhs));
        std::swap(lhs.comp, rhs.comp);
    }
};

template<typename T>
OrderedLinkedList<T>::OrderedLinkedList(const OrderedLinkedList<T>::CompareFunction& comp)
    : comp(comp)
{}

template<typename T>
void OrderedLinkedList<T>::push(const T& element)
{
    if (this->length == 0) {
        this->first = this->last = this->cache.node = new Node { element, nullptr };
        this->length++;
        return;
    }

    Node *prev = nullptr, *cur = this->first;
    for (size_t idx = 0; cur; idx++) {
        if (comp(element, cur->value)) {
            if (prev) {
                prev->next = new Node { element, cur };
                if (idx <= this->cache.idx) {
                    this->cache.idx++;
                }
                this->cache.idx = 0;
                this->cache.node = this->first;
            } else {
                this->first = new Node { element, this->first };
                this->cache.idx++;
            }
            this->length++;
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    this->last->next = new Node { element, nullptr };
    this->last = this->last->next;

    this->length++;
}

template<typename T>
bool OrderedLinkedList<T>::operator==(const OrderedLinkedList &other) const
{
    return LinkedList<T>::operator==(other);
}

template<typename T>
bool OrderedLinkedList<T>::operator!=(const OrderedLinkedList &other) const
{
    return LinkedList<T>::operator!=(other);
}

#endif // __ORDERED_LINKEDLIST_H__

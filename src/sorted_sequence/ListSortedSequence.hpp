//
// Created by Данила Фомин on 24.11.2017.
//

#ifndef LAB2_LISTSORTEDSEQUENCE_HPP
#define LAB2_LISTSORTEDSEQUENCE_HPP

#include <stdexcept>
#include "SortedSequence.hpp"

namespace lab2::sequence {

    template <class ElementType>
    class ListSortedSequence : public SortedSequence<ElementType> {

    public:

        // constuctors

        ListSortedSequence(ComparatorType<ElementType> comparator = less<ElementType>()) noexcept
                : _comparator{comparator}, _len{0}, _head{nullptr} {}

        ListSortedSequence(const ListSortedSequence<ElementType>& obj)
                : _comparator{obj._comparator}, _len{obj._len}, _head{nullptr} {
            if(obj._head) {
                _head = new TNode(*obj._head);
                for(auto nodePtr = _head; nodePtr -> _next; nodePtr = nodePtr -> _next) {
                    nodePtr -> _next = new TNode(*(nodePtr -> _next));
                }
            }
        }

        ListSortedSequence(ListSortedSequence<ElementType>&& obj) noexcept
                : _comparator{obj._comparator}, _len{obj._len}, _head{obj._head} {
            obj._comparator = less<ElementType>();
            obj._len = 0;
            obj._head = nullptr;
        }


        // operators =

        ListSortedSequence<ElementType>& operator= (const ListSortedSequence<ElementType>& rhs) {
            if (this != &rhs) {
                clear();
                _len = rhs._len;
                _head = nullptr;
                _comparator = rhs._comparator;
                if(rhs._head) {
                    _head = new TNode(*rhs._head);
                    for(auto nodePtr = _head; nodePtr -> _next; nodePtr = nodePtr -> _next) {
                        nodePtr -> _next = new TNode(*(nodePtr -> _next));
                    }
                }
            }
            return *this;
        }

        ListSortedSequence<ElementType>& operator= (ListSortedSequence<ElementType>&& rhs) {
            if (this != &rhs) {
                clear();
                _len = rhs._len;
                _comparator = rhs._comparator;
                _head = rhs._head;

                rhs._head = nullptr;
                rhs._comparator = less<ElementType>();
                rhs._len = 0;
            }
            return *this;
        }

        // destructor

        ~ListSortedSequence() override { clear(); }

        // overrided methods

        size_t getLength() const noexcept override { return _len; }

        bool isEmpty() const noexcept override { return !_len; }

        operator bool() const noexcept override { return _len; }

        size_t indexOf(const ElementType& element) const override {
            size_t index = 0;
            for (auto nodePtr = _head; nodePtr; nodePtr = nodePtr -> _next) {
                if (nodePtr -> _value == element) { return index; }
                ++index;
            }
            throw std::out_of_range("element is missing");
        }

        ElementType get(size_t index) const override {
            if (_head && index >= 0 && index < _len) {
                auto nodePtr = _head;
                for (auto i = 0; i < index; ++i) {
                    nodePtr = nodePtr -> _next;
                }
                return nodePtr -> _value;
            }
            throw std::out_of_range("index is out of range");
        }

        ElementType operator[](size_t index) const override { return get(index); }

        ElementType getFirst() const override {
            if (_head) { return _head -> _value; }
            throw std::out_of_range("sequence is empty");
        }

        ElementType getLast() const override {
            if (_head) {
                auto nodePtr = _head;
                for (auto i = 0; i < _len - 1; ++i) {
                    nodePtr = nodePtr -> _next;
                }
                return nodePtr -> _value;
            }
            throw std::out_of_range("sequence is empty");
        }

        void add(const ElementType& element) override {
            if (_head) {
                if (_comparator(element, _head -> _value)) {
                    _head = new TNode(element, _head);
                    ++_len;
                    return;
                }
                auto nodePtr = _head;
                for (; nodePtr -> _next; nodePtr = nodePtr -> _next) {
                    if (_comparator(element, nodePtr -> _next -> _value)) { break; }
                }
                nodePtr -> _next = new TNode(element, nodePtr -> _next);
                ++_len;
            } else {
                _head = new TNode(element);
                ++_len;
            }
        }

        void add(ElementType&& element) override {
            if (_head) {
                if (_comparator(element, _head -> _value)) {
                    _head = new TNode(element, _head);
                    ++_len;
                    return;
                }
                auto nodePtr = _head;
                for (; nodePtr -> _next; nodePtr = nodePtr -> _next) {
                    if (_comparator(element, nodePtr -> _next -> _value)) { break; }
                }
                nodePtr -> _next = new TNode(element, nodePtr -> _next);
                ++_len;
            } else {
                _head = new TNode(element);
                ++_len;
            }
        }

        void remove(const ElementType& element) noexcept override {
            if (_head && (_head -> _value == element)) {
                auto tempPtr = _head;
                _head = _head -> _next;
                delete tempPtr;
                --_len;
                return;
            }
            for (auto nodePtr = _head; nodePtr; nodePtr = nodePtr -> _next) {
                if (nodePtr ->_next && (nodePtr -> _next -> _value == element)) {
                    auto tempPtr = nodePtr -> _next;
                    nodePtr -> _next = nodePtr -> _next ->_next;
                    delete tempPtr;
                    --_len;
                    return;
                }
            }
        }

        void clear(ComparatorType<ElementType> newComparator = less<ElementType>()) noexcept override {
            for (auto nodePtr = _head; nodePtr;) {
                auto tempPtr = nodePtr;
                nodePtr = nodePtr -> _next;
                delete tempPtr;
            }
            _head = nullptr;
            _len = 0;
            _comparator = newComparator;
        }

        SortedSequence<ElementType>* getSubsequence(size_t startIndex, size_t endIndex) const override {
            if (startIndex >= 0 && endIndex < _len && startIndex <= endIndex) {
                auto nodePtr = _head;
                size_t i = 0;
                for (; i < startIndex; ++i) {
                    nodePtr = nodePtr -> _next;
                }
                auto tempObj = new ListSortedSequence<ElementType>(_comparator);

                tempObj -> _head = new TNode(*nodePtr);
                auto nodePtrForTemp = tempObj -> _head;
                ++(tempObj -> _len);


                for (; i < endIndex; nodePtrForTemp = nodePtrForTemp -> _next) {
                    if (nodePtrForTemp -> _next) {
                        nodePtrForTemp -> _next = new TNode(*(nodePtrForTemp -> _next));
                    }
                    ++i;
                    ++(tempObj -> _len);
                }
                return tempObj;
            }
            throw std::out_of_range("indices are out of range");
        }

        SortedSequence<ElementType>* copy() const override {
            return new ListSortedSequence<ElementType>{*this};
        }

    private:

        struct TNode {

            TNode* _next;

            ElementType _value;

            TNode(const ElementType& value, TNode* next = nullptr) noexcept
                    : _value{value}, _next{next} {}

            TNode(ElementType&& value, TNode* next = nullptr) noexcept
                    : _value{value}, _next{next} {}

        };

        TNode* _head;

        size_t _len;

        ComparatorType<ElementType> _comparator;

    };

}

#endif //LAB2_LISTSORTEDSEQUENCE_HPP

//
// Created by Данила Фомин on 24.11.2017.
//

#ifndef LAB2_ARRAYSORTEDSEQUENCE_HPP
#define LAB2_ARRAYSORTEDSEQUENCE_HPP

#include <stdexcept>
#include "SortedSequence.hpp"

namespace lab2::sequence {

    template <class ElementType>
    class ArraySortedSequence : public SortedSequence<ElementType> {
    public:
        // constructors

        ArraySortedSequence(ComparatorType<ElementType> comparator = less<ElementType>()) noexcept
                : _len{0}, _arrPtr{nullptr}, _comparator{comparator} {}

        ArraySortedSequence(const ArraySortedSequence<ElementType>& obj)
                : _len{0}, _arrPtr{nullptr}, _comparator{obj._comparator} {
            if (obj._len) {
                _arrPtr = new ElementType[obj._len];
                _len = obj._len;
                for (auto i = 0; i < _len; ++i) {_arrPtr[i] = obj._arrPtr[i];}
            }
        }

        ArraySortedSequence(ArraySortedSequence<ElementType>&& obj) noexcept
                : _len{obj._len}, _arrPtr{obj._arrPtr}, _comparator{obj._comparator} {
            obj._len = 0;
            obj._comparator = less<ElementType>();
            obj._arrPtr = nullptr;
        }

        // operators =

        ArraySortedSequence<ElementType>& operator= (const ArraySortedSequence<ElementType>& rhs) {
            if (this != &rhs) {
                clear();
                if (rhs._len) {
                    _arrPtr = new ElementType[rhs._len];
                    _len = rhs._len;
                    for (auto i = 0; i < _len; ++i) {_arrPtr[i] = rhs._arrPtr[i];}
                }
            }
            return *this;
        }

        ArraySortedSequence<ElementType>& operator = (ArraySortedSequence<ElementType>&& rhs) noexcept {
            if (this != &rhs) {
                clear();
                _len = rhs._len;
                _arrPtr = rhs._arrPtr;
                _comparator = rhs._comparator();

                rhs._len = 0;
                rhs._comparator = less<ElementType>();
                rhs._arrPtr = nullptr;
            }
            return *this;
        }

        // destructor

        ~ArraySortedSequence() override { clear(); }

        // overrided methods

        size_t size() const noexcept override { return _len; }

        bool isEmpty() const noexcept override { return !_len; }

        operator bool() const noexcept override { return _len; }

        ElementType get(size_t index) const override {
            if (index >= 0 && index < _len) { return _arrPtr[index]; }
            throw std::out_of_range("index is out of range");
        }

        ElementType operator[](size_t index) const override { return get(index); }

        ElementType getFirst() const override {
            if (_len) { return *_arrPtr; }
            throw std::out_of_range("sequence is empty");
        }

        ElementType getLast() const override {
            if (_len) { return _arrPtr[_len - 1]; }
            throw std::out_of_range("sequence is empty");
        }

        size_t indexOf(const ElementType& element) const override {
            if (_len) {
                size_t le = 0, re = _len - 1;
                while (le != re) {
                    size_t pivot = (le + re) / 2;
                    if (_arrPtr[pivot] == element) { return pivot; }
                    if (_comparator(element, _arrPtr[pivot])) { re = pivot; }
                    else { le = pivot + 1; }
                }
                if (_arrPtr[le] == element) { return le; }
            }
            throw std::out_of_range("element is missing");
        }

        SortedSequence<ElementType>* getSubsequence(size_t startIndex, size_t endIndex) const override {
            if (startIndex >= 0 && endIndex < _len && startIndex <= endIndex) {
                auto tempObjPtr = new ArraySortedSequence<ElementType>(_comparator);
                tempObjPtr -> _len = endIndex - startIndex + 1;
                tempObjPtr -> _arrPtr = new ElementType[tempObjPtr -> _len];
                for (auto i = startIndex; i <= endIndex; ++i) {
                    tempObjPtr -> _arrPtr[i - startIndex] = _arrPtr[i];
                }
                return tempObjPtr;
            }
            throw std::out_of_range("indices are out of range");
        }

        SortedSequence<ElementType>* copy() const override {
            return new ArraySortedSequence<ElementType>{*this};
        }

        void add(const ElementType& element) override {
            auto tempArr = new ElementType[_len + 1];
            size_t difference = 0, i = 0;
            for (; i < _len; ++i) {
                if (!difference && _comparator(element, _arrPtr[i])) {
                    tempArr[i] = element;
                    ++difference;
                }
                tempArr[i + difference] = std::move(_arrPtr[i]);
            }
            if (!difference) { tempArr[i] = element; }
            ++_len;
            delete [] _arrPtr;
            _arrPtr = tempArr;
        }

        void add(ElementType&& element) override {
            auto tempArr = new ElementType[_len + 1];
            size_t difference = 0, i = 0;
            for (; i < _len; ++i) {
                if (!difference && _comparator(element, _arrPtr[i])) {
                    tempArr[i] = std::move(element);
                    ++difference;
                }
                tempArr[i + difference] = std::move(_arrPtr[i]);
            }
            if (!difference) { tempArr[i] = std::move(element); }
            ++_len;
            delete [] _arrPtr;
            _arrPtr = tempArr;
        }

        void remove(const ElementType& element) override {
            if (_len) {
                auto tempArr = new ElementType[_len - 1];
                int difference = 0;
                for (auto i = 0; i < _len; ++i) {
                    if (difference || !(_arrPtr[i] == element)) {
                        tempArr[i + difference] = std::move(_arrPtr[i]);
                    } else { --difference; }
                }
                _len += difference;
                delete [] _arrPtr;
                _arrPtr = tempArr;
            }
        }

        void clear(ComparatorType<ElementType> newComparator = less<ElementType>()) noexcept override {
            delete [] _arrPtr;
            _arrPtr = nullptr;
            _len = 0;
            _comparator = newComparator;
        }


    private:

        size_t _len;

        ElementType* _arrPtr;

        ComparatorType<ElementType> _comparator;

    };

}

#endif //LAB2_ARRAYSORTEDSEQUENCE_HPP

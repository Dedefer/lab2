//
// Created by Данила Фомин on 24.11.2017.
//

#ifndef LAB2_SORTEDSEQUENCE_HPP
#define LAB2_SORTEDSEQUENCE_HPP

#include <cstddef>
#include "../lab2_namespace.hpp"


namespace lab2::sequence {

        template<class ElementType>
        class SortedSequence {
        public:

            virtual size_t getLength() const noexcept = 0;

            virtual bool isEmpty() const noexcept = 0;

            virtual operator bool() const noexcept = 0;

            virtual size_t indexOf(const ElementType &element) const = 0;

            virtual ElementType get(size_t index) const = 0;

            virtual ElementType operator[](size_t index) const = 0;

            virtual ElementType getFirst() const = 0;

            virtual ElementType getLast() const = 0;

            virtual void add(const ElementType& element) = 0;

            virtual void add(ElementType&& element) = 0;

            virtual void remove(const ElementType& element) = 0;

            virtual void clear(ComparatorType<ElementType> newComparator = less<ElementType>()) noexcept = 0;

            virtual SortedSequence<ElementType>* getSubsequence(size_t startIndex, size_t endIndex) const = 0;

            virtual SortedSequence<ElementType>* copy() const = 0;

            virtual ~SortedSequence() = default;

        };
  }


#endif //LAB2_SORTEDSEQUENCE_HPP

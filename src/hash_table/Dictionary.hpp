//
// Created by Данила Фомин on 25.11.2017.
//

#ifndef LAB2_DICTIONARY_HPP
#define LAB2_DICTIONARY_HPP


#include <cstddef>
#include "../lab2_namespace.hpp"

namespace lab2::hash_table {

    template <class KeyType, class ElementType>
    class Dictionary {
    public:

        using ConstKeyRef = const KeyType&;

        using ConstElementRef = const ElementType&;

        virtual size_t getCount() const noexcept = 0;

        virtual ElementType get(ConstKeyRef key) const = 0;

        virtual ElementType operator[](ConstKeyRef key) const = 0;

        virtual bool check(ConstKeyRef key) const noexcept = 0;

        virtual void add(ConstKeyRef key, ConstElementRef element) = 0;

        virtual void clear(ComparatorType<KeyType> comparator = equal<KeyType>()) noexcept = 0;

        virtual ~IDictionary() = default;

    };

}

#endif //LAB2_DICTIONARY_HPP

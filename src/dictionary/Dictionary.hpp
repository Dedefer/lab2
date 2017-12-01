//
// Created by Данила Фомин on 25.11.2017.
//

#ifndef LAB2_DICTIONARY_HPP
#define LAB2_DICTIONARY_HPP


#include <cstddef>
#include "../lab2_namespace.hpp"


// comparator ==, comparator <, hash function, etc
// will be immutable and passed in constructors.
namespace lab2::dictionary {

    template <class KeyType, class ElementType>
    class Dictionary {

    public:

        using ConstKeyRef = const KeyType&;

        using ConstElementRef = const ElementType&;

        virtual size_t size() const noexcept = 0;

        virtual ElementType get(ConstKeyRef key) const = 0;

        virtual ElementType operator[](ConstKeyRef key) const = 0;

        virtual bool check(ConstKeyRef key) const noexcept = 0;

        virtual void add(ConstKeyRef key, ConstElementRef element) = 0;

        virtual void remove(ConstKeyRef key) noexcept = 0;

        virtual void clear() noexcept = 0;

        virtual ~Dictionary() = default;

    };

}

#endif //LAB2_DICTIONARY_HPP

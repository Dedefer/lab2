//
// Created by Данила Фомин on 24.11.2017.
//

#ifndef LAB2_LAB2_NAMESPACE_HPP
#define LAB2_LAB2_NAMESPACE_HPP

#include <functional>

namespace lab2 {

    template <class T>
    using ComparatorType = std::function<bool(const T&, const T&)>;

    template <class T>
    struct less {
        constexpr bool operator()(const T& lhs, const T& rhs) {
            return lhs < rhs;
        }
    };

    template <class T>
    struct equal {
        constexpr bool operator()(const T& lhs, const T& rhs) const {
            return lhs == rhs;
        }
    };

    namespace dictionary {

        template <class T>
        using HasherType = std::function<size_t(const T&)>;

    }

}

#endif //LAB2_LAB2_NAMESPACE_HPP

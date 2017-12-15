//
// Created by Данила Фомин on 15.12.2017.
//

#ifndef LAB2_TEST_HEADER_HPP
#define LAB2_TEST_HEADER_HPP

#include <vector>
#include <string>
#include <random>
#include <algorithm>

namespace lab2::test {

    std::string decoration(const std::string &str) {
        return "===========\n" + str + "\n===========\n";
    }

    std::vector<int> shuffledArray(int len) {
        std::vector<int> result(len);
        for (auto j = 0; j < len; ++j) { result[j] = j + 1; }
        std::random_device rGen;
        std::shuffle(result.begin(), result.end(), rGen);
        return std::move(result);
    }

    void inc(int& i) { ++i; }

}

#endif //LAB2_TEST_HEADER_HPP

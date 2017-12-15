//
// Created by Данила Фомин on 16.12.2017.
//

#ifndef LAB2_ARRAYSORTEDSEQUENCESEARCHSPEEDTEST_HPP
#define LAB2_ARRAYSORTEDSEQUENCESEARCHSPEEDTEST_HPP

#include "test_header.hpp"
#include "../src/sorted_sequence/ArraySortedSequence.hpp"
#include <ctime>

namespace lab2::test::ArraySequenceSearchSpeedTest {
    using lab2::sequence::ArraySortedSequence;
    using lab2::test::shuffledArray;

    size_t linearSearch(ArraySortedSequence<int>& seq, int element) {
        auto size = seq.size();
        for (auto i = 0; i < size; ++i) {
            if (element == seq[i]) return i;
        }
        throw std::out_of_range{"element is missing"};
    }

    void compareLinearAndBinary(std::ostream& stream, size_t minSize, size_t maxSize,  size_t step) {
        stream << "Size\t" << "linear\t" << "binary\n";
        for (auto currentSize = minSize; currentSize <= maxSize; currentSize += step) {
            ArraySortedSequence<int> testSeq;
            for (auto i = 1; i <= currentSize; ++i) { testSeq.add(i); }
            auto arr = shuffledArray(currentSize);
            double avrLinearTime = 0;
            double avrBinaryTime = 0;
            for (auto item : arr) {
                auto time1 = clock();
                linearSearch(testSeq, item);
                auto time2 = clock();
                avrLinearTime += ((double)time2 - time1)/CLOCKS_PER_SEC;

                time1 = clock();
                testSeq.indexOf(item);
                time2 = clock();
                avrBinaryTime += ((double)time2 - time1)/CLOCKS_PER_SEC;
            }
            avrBinaryTime /= currentSize;
            avrLinearTime /= currentSize;
            stream << currentSize << "\t" << avrLinearTime << "\t" << avrBinaryTime << "\n";
        }
    }
}


#endif //LAB2_ARRAYSORTEDSEQUENCESEARCHSPEEDTEST_HPP

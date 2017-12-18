#include <iostream>
#include <fstream>
#include "tests/RBTreeTest.hpp"
#include "tests/HashTableTest.hpp"
#include "tests/SortedSequenceTest.hpp"
#include "src/sorted_sequence/ListSortedSequence.hpp"
#include "src/sorted_sequence/ArraySortedSequence.hpp"
#include "tests/ArraySortedSequenceSearchSpeedTest.hpp"


int main() {
    using namespace lab2::test;
    using namespace lab2::sequence;
    auto file = std::ofstream("../testlog.txt");
    if (file) {
        SortedSequenceTest<ListSortedSequence<int>>::testScript(file, "LIST");
        SortedSequenceTest<ArraySortedSequence<int>>::testScript(file, "ARRAY");
        lab2::test::HashTableTest::testScript(file);
        lab2::test::RBTreeTest::testScript(file);
    }
    /*
    auto file = std::ofstream("../SpeedTest.txt");
    if (file) {
        ArraySequenceSearchSpeedTest::compareLinearAndBinary(file, 10000, 50000, 500);
    }
    */
    return 0;
}
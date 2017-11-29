#include <iostream>
#include "src/sorted_sequence/ArraySortedSequence.hpp"
#include "src/sorted_sequence/ListSortedSequence.hpp"


bool grater(const int& a, const int& b) {
    return a > b;
}

using namespace lab2::sequence;

int main() {
    SortedSequence<int>* a = new ListSortedSequence<int>(grater);
    a -> add(5);
    a -> add(8);
    a -> add(4);
    a -> add(5);
    a -> add(5);

    for (int i = 0, size = a -> getLength(); i < size; ++i) {
        std::cout << a -> get(i) << std::endl;
    }

    try {
        std::cout << std::endl << a -> indexOf(5);
    } catch (std::exception& p) {std::cout << p.what();}
    return 0;
}
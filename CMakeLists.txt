cmake_minimum_required(VERSION 3.8)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp src/sorted_sequence/SortedSequence.hpp src/sorted_sequence/ArraySortedSequence.hpp src/lab2_namespace.hpp src/sorted_sequence/ListSortedSequence.hpp src/dictionary/Dictionary.hpp src/dictionary/hash_table/HashTable.hpp)
add_executable(lab2 ${SOURCE_FILES})
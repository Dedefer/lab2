//
// Created by Данила Фомин on 15.12.2017.
//

#ifndef LAB2_HASHTABLETEST_HPP
#define LAB2_HASHTABLETEST_HPP

#include <iostream>


#include "../src/dictionary/HashTable.hpp"
#include "test_header.hpp"

namespace lab2::test::HashTableTest {

    using namespace lab2::dictionary;

    int stressTestInsert(std::ostream &stream, int maxCountOfElements, int countOfCycles) {
        std::string logSrting = std::string{"HASHTABLE STRESS TEST INSERTION\n"} +
                                "(inserts " + std::to_string(maxCountOfElements) +
                                " shuffled ints to HashTable\n" +
                                "and after each insertion checks\n" +
                                "size() <= capacity()\n";
        int testPassed;
        int globalTestPassed = 0;
        for (auto i = 0; i < countOfCycles; ++i) {
            testPassed = 0;
            HashTable<int, int> testTable;
            auto insertSeq = shuffledArray(maxCountOfElements);
            for (auto item : insertSeq) {
                testTable.add(item, item);
                if (testTable.capacity() >= testTable.size()) { ++testPassed; }
            }
            if (testPassed == maxCountOfElements) { ++globalTestPassed; }
            logSrting += std::to_string(i + 1) + " cycle : " +
                         std::to_string(testPassed) + "/" + std::to_string(maxCountOfElements) + '\n';
        }
        logSrting += std::to_string(globalTestPassed) + "/" + std::to_string(countOfCycles) + " : OK";
        stream << decoration(logSrting);
        return globalTestPassed == countOfCycles;
    }

    int testRemoveNotExist(std::ostream& stream) {
        std::string logString = std::string{"HASHTABLE REMOVE TEST 1\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added (key = i, value = 0) i = 1..100\n";
        test.remove(1000);
        logString += "remove(1000) called\n";
        auto size = test.size();
        logString += "HashTable.size() = " + std::to_string(size) +
                     " expected 100 : " + ((size == 100) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 100;
    }

    int testRemoveExist(std::ostream& stream) {
        std::string logString = std::string{"HASHTABLE REMOVE TEST 2\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added (key = i, value = 0) i = 1..100\n";
        test.remove(100);
        logString += "remove(100) called\n";
        auto size = test.size();
        logString += "HashTable.size() = " + std::to_string(size) +
                     " expected 99 : " + ((size == 99) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 99;
    }

    int testSizeConstruct(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE SIZE TEST 1\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto size = test.size();
        logString += "HashTable.size() = " + std::to_string(size) +
                     " expected 0 : " + ((size == 0) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 0;
    }

    int testSizeAdd100(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE SIZE TEST 2\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added 100 elements\n";
        auto size = test.size();
        logString += "HashTable.size() = " + std::to_string(size) +
                     " expected 100 : " + ((size == 100) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 100;
    }

    int testGetEmpty(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE GET TEST 1\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        logString += "trying to get(0)\nexpected exception : ";
        bool exceptionAccepted = false;
        try {
            test.get(0);
        } catch (std::exception &) { exceptionAccepted = true; }
        logString += exceptionAccepted ? "OK" : "PROBLEM";
        stream << decoration(logString);
        return exceptionAccepted;
    }

    int testGetExist(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE GET TEST 2\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        logString += "added (key = 5, value = 13)\n";
        test.add(5, 13);
        auto got = test.get(5);
        logString += "HashTable.get(5) = " + std::to_string(got) +
                     " expected 13 : " + ((got == 13) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return got == 13;
    }

    int testCapacityConstruct(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE CAPACITY TEST 1\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto size = test.capacity();
        logString += "HashTable.capacity() = " + std::to_string(size) +
                     " expected 384 : " + ((size == 384) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 384;
    }

    int testCapacityAdd1000(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE CAPACITY TEST 2\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto v = shuffledArray(1000);
        for (auto i : v) { test.add(i, 0); }
        logString += "added 1000 elements\n";
        auto size = test.capacity();
        logString += "HashTable.capacity() = " + std::to_string(size) +
                     " expected more than 1000 : " + ((size >= 1000) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size >= 1000;
    }

    int testCheckEmpty(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE CHECK TEST 1\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto exist = test.check(1);
        logString += "HashTable.check(1) = " + std::to_string(exist) +
                     " expected false : " + ((!exist) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return !exist;
    }

    int testCheckExist(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE CHECK TEST 2\n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        logString += "added (key = 5, value = 13)\n";
        test.add(5, 13);
        auto exist = test.check(5);
        logString += "HashTable.check(5) = " + std::to_string(exist) +
                     " expected true : " + ((exist) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return exist;
    }

    int testClear(std::ostream &stream) {
        std::string logString = std::string{"HASHTABLE CLEAR TEST \n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added 100 elements\n";
        test.clear();
        logString += "clear() called\n";
        auto size = test.size();
        logString += "HashTable.size() = " + std::to_string(size) +
                     " expected 0 : " + ((size == 0) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 0;
    }

    int testMap(std::ostream& stream) {
        std::string logString = std::string{"HASHTABLE MAP TEST \n"}
                                + "new HashTable constructed\n";
        HashTable<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added (key = i, value = 0) i = 1..100\n";
        test.map(inc);
        logString += "map(inc) called\n";
        logString += "(inc() increments value)\n";
        auto got = test.get(5);
        logString += "HashTable.get(5) = " + std::to_string(got) +
                     " expected 1 : " + ((got == 1) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return got == 1;

    }

    void testScript(std::ostream& stream) {
        stream << "#######################\nHASHTABLE TEST\n";
        int testsPassed = 0;
        testsPassed += testSizeConstruct(stream);
        testsPassed += testSizeAdd100(stream);
        testsPassed += testCapacityConstruct(stream);
        testsPassed += testCapacityAdd1000(stream);
        testsPassed += testCheckEmpty(stream);
        testsPassed += testCheckExist(stream);
        testsPassed += testGetEmpty(stream);
        testsPassed += testGetExist(stream);
        testsPassed += testClear(stream);
        testsPassed += testMap(stream);
        testsPassed += stressTestInsert(stream, 10000, 5);
        testsPassed += testRemoveNotExist(stream);
        testsPassed += testRemoveExist(stream);
        stream << testsPassed << "/13 : OK\n#######################";
    }
}

#endif //LAB2_HASHTABLETEST_HPP

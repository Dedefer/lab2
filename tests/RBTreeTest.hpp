//
// Created by Данила Фомин on 08.12.2017.
//

#ifndef LAB2_RBTREETEST_HPP
#define LAB2_RBTREETEST_HPP

#include <iostream>


#include "../src/dictionary/RBTree.hpp"
#include "test_header.hpp"

namespace lab2::test::RBTreeTest {

    using namespace lab2::dictionary;

    struct DepthChecker {

        // this shit acting right only if keys == elements
        // and all keys > 0 and comparator is <

        std::vector<int> &depth;

        int index;

        int previousKey;

        int currentDepth = 0;

        void operator()(int &el) {
            if (!(el < previousKey)) {
                depth.push_back(currentDepth);
                index = depth.size() - 1;
            }
            ++depth[index];
            previousKey = el;
            ++currentDepth;
        }

        DepthChecker(std::vector<int> &initDepth) : depth{initDepth}, index{-1}, previousKey{0} {}

    };

    bool checkDepthRatio(RBTree<int, int> &tree) {
        std::vector<int> vOfDepths;
        tree.map(DepthChecker{vOfDepths});
        auto minmax = std::minmax_element(vOfDepths.begin(), vOfDepths.end());
        return ((double) *(minmax.second) / *minmax.first) <= 2;
    }

    int stressTestInsert(std::ostream &stream, int maxCountOfElements, int countOfCycles) {
        std::string logSrting = std::string{"RBTREE STRESS TEST INSERTION\n"} +
                                "(inserts " + std::to_string(maxCountOfElements) + " shuffled ints to RBTree\n" +
                                "and after each insertion checks that path\n" +
                                "from the root to the farthest leaf\n" +
                                "is no more than twice as long as the path\n" +
                                "from the root to the nearest leaf)\n";
        int testPassed;
        int globalTestPassed = 0;
        for (auto i = 0; i < countOfCycles; ++i) {
            testPassed = 0;
            RBTree<int, int> testTree;
            auto insertSeq = shuffledArray(maxCountOfElements);
            for (auto item : insertSeq) {
                testTree.add(item, item);
                if (checkDepthRatio(testTree)) { ++testPassed; }
            }
            if (testPassed == maxCountOfElements) { ++globalTestPassed; }
            logSrting += std::to_string(i + 1) + " cycle : " +
                         std::to_string(testPassed) + "/" + std::to_string(maxCountOfElements) + '\n';
        }
        logSrting += std::to_string(globalTestPassed) + "/" + std::to_string(countOfCycles) + " : OK";
        stream << decoration(logSrting);
        return globalTestPassed == countOfCycles;
    }

    int stressTestRemove(std::ostream &stream, int maxCountOfElements, int countOfCycles) {
        std::string logSrting = std::string{"RBTREE STRESS TEST REMOVE\n"} +
                                "(inserts " + std::to_string(maxCountOfElements) + " shuffled ints to RBTree\n" +
                                "then removes them in random order\n" +
                                "and after each remove checks that path\n" +
                                "from the root to the farthest leaf\n" +
                                "is no more than twice as long as the path\n" +
                                "from the root to the nearest leaf)\n";
        int testPassed;
        int globalTestPassed = 0;
        for (auto i = 0; i < countOfCycles; ++i) {
            testPassed = 0;
            RBTree<int, int> testTree;
            auto Seq = shuffledArray(maxCountOfElements);
            for (auto item : Seq) {
                testTree.add(item, item);
            }
            Seq = shuffledArray(maxCountOfElements);
            for (auto item : Seq) {
                if (checkDepthRatio(testTree)) { ++testPassed; }
                testTree.remove(item);
            }
            if (testPassed == maxCountOfElements) { ++globalTestPassed; }
            logSrting += std::to_string(i + 1) + " cycle : " +
                         std::to_string(testPassed) + "/" + std::to_string(maxCountOfElements) + '\n';
        }
        logSrting += std::to_string(globalTestPassed) + "/" + std::to_string(countOfCycles) + " : OK";
        stream << decoration(logSrting);
        return globalTestPassed == countOfCycles;

    }

    int testSizeConstruct(std::ostream &stream) {
        std::string logString = std::string{"RBTREE SIZE TEST 1\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        auto size = test.size();
        logString += "RBTree.size() = " + std::to_string(size) +
                     " expected 0 : " + ((size == 0) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 0;
    }

    int testSizeAdd100(std::ostream &stream) {
        std::string logString = std::string{"RBTREE SIZE TEST 2\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added 100 elements\n";
        auto size = test.size();
        logString += "RBTree.size() = " + std::to_string(size) +
                     " expected 100 : " + ((size == 100) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 100;
    }

    int testGetEmpty(std::ostream &stream) {
        std::string logString = std::string{"RBTREE GET TEST 1\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
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
        std::string logString = std::string{"RBTREE GET TEST 2\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        logString += "added (key = 5, value = 13)\n";
        test.add(5, 13);
        auto got = test.get(5);
        logString += "RBTree.get(5) = " + std::to_string(got) +
                     " expected 13 : " + ((got == 13) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return got == 13;
    }

    int testCapacityConstruct(std::ostream &stream) {
        std::string logString = std::string{"RBTREE CAPACITY TEST 1\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        auto size = test.capacity();
        logString += "RBTree.capacity() = " + std::to_string(size) +
                     " expected 0 : " + ((size == 0) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 0;
    }

    int testCapacityAdd100(std::ostream &stream) {
        std::string logString = std::string{"RBTREE CAPACITY TEST 2\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added 100 elements\n";
        auto size = test.capacity();
        logString += "RBTree.capacity() = " + std::to_string(size) +
                     " expected 100 : " + ((size == 100) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 100;
    }

    int testCheckEmpty(std::ostream &stream) {
        std::string logString = std::string{"RBTREE CHECK TEST 1\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        auto exist = test.check(1);
        logString += "RBTree.check(1) = " + std::to_string(exist) +
                     " expected false : " + ((!exist) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return !exist;
    }

    int testCheckExist(std::ostream &stream) {
        std::string logString = std::string{"RBTREE CHECK TEST 2\n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        logString += "added (key = 5, value = 13)\n";
        test.add(5, 13);
        auto exist = test.check(5);
        logString += "RBTree.check(5) = " + std::to_string(exist) +
                     " expected true : " + ((exist) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return exist;
    }

    int testClear(std::ostream &stream) {
        std::string logString = std::string{"RBTREE CLEAR TEST \n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added 100 elements\n";
        test.clear();
        logString += "clear() called\n";
        auto size = test.size();
        logString += "RBTree.size() = " + std::to_string(size) +
                     " expected 0 : " + ((size == 0) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return size == 0;
    }

    int testMap(std::ostream& stream) {
        std::string logString = std::string{"RBTREE MAP TEST \n"}
                                + "new RBTree constructed\n";
        RBTree<int, int> test;
        auto v = shuffledArray(100);
        for (auto i : v) { test.add(i, 0); }
        logString += "added (key = i, value = 0) i = 1..100\n";
        test.map(inc);
        logString += "map(inc) called\n";
        logString += "(inc() increments value)\n";
        auto got = test.get(5);
        logString += "RBTree.get(5) = " + std::to_string(got) +
                     " expected 1 : " + ((got == 1) ? "OK" : "PROBLEM");
        stream << decoration(logString);
        return got == 1;

    }

    void testScript(std::ostream& stream) {
        stream << "#######################\nRBTREE TEST\n";
        int testsPassed = 0;
        testsPassed += testSizeConstruct(stream);
        testsPassed += testSizeAdd100(stream);
        testsPassed += testCapacityConstruct(stream);
        testsPassed += testCapacityAdd100(stream);
        testsPassed += testCheckEmpty(stream);
        testsPassed += testCheckExist(stream);
        testsPassed += testGetEmpty(stream);
        testsPassed += testGetExist(stream);
        testsPassed += testClear(stream);
        testsPassed += testMap(stream);
        testsPassed += stressTestInsert(stream, 10000, 5);
        testsPassed += stressTestRemove(stream, 10000, 5);
        stream << testsPassed << "/12 : OK\n#######################";
    }
}


#endif //LAB2_RBTREETEST_HPP

//
// Created by Данила Фомин on 15.12.2017.
//

#ifndef LAB2_SORTEDSEQUENCETEST_HPP
#define LAB2_SORTEDSEQUENCETEST_HPP

#include "test_header.hpp"
#include "../src/sorted_sequence/SortedSequence.hpp"

namespace lab2::test {

    using namespace lab2::sequence;
    template <class Sequence>
    class SortedSequenceTest {
    private:

        static bool checkOrder(const SortedSequence<int>& seq) {
            auto len = seq.size();
            auto ok = true;
            for (auto i = 0; i < (len - 1); ++i) {
                ok = ok && (seq[i] <= seq[i+1]);
            }
            return ok;
        }

    public:
        static int stressTestInsert(std::ostream &stream, int maxCountOfElements, int countOfCycles) {
            std::string logSrting = std::string{"SORTED SEQUENCE STRESS TEST INSERTION\n"} +
                                    "(inserts " + std::to_string(maxCountOfElements) + " shuffled ints to SortedSequence\n" +
                                    "and after each insertion checks that each\n" +
                                    "next element not less that previous\n";
            int testPassed;
            int globalTestPassed = 0;
            for (auto i = 0; i < countOfCycles; ++i) {
                testPassed = 0;
                SortedSequence<int>&& testSequence = Sequence();
                auto insertSeq = shuffledArray(maxCountOfElements);
                for (auto item : insertSeq) {
                    testSequence.add(item);
                    if (checkOrder(testSequence)) { ++testPassed; }
                }
                if (testPassed == maxCountOfElements) { ++globalTestPassed; }
                logSrting += std::to_string(i + 1) + " cycle : " +
                             std::to_string(testPassed) + "/" + std::to_string(maxCountOfElements) + '\n';
            }
            logSrting += std::to_string(globalTestPassed) + "/" + std::to_string(countOfCycles) + " : OK";
            stream << decoration(logSrting);
            return globalTestPassed == countOfCycles;
        }

        static int testRemoveNotExist(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE REMOVE TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            auto v = shuffledArray(100);
            for (auto i : v) { test.add(i); }
            logString += "added (key = i, value = 0) i = 1..100\n";
            test.remove(1000);
            logString += "remove(1000) called\n";
            auto size = test.size();
            logString += "SortedSequence.size() = " + std::to_string(size) +
                         " expected 100 : " + ((size == 100) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return size == 100;
        }

        static int testRemoveExist(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE REMOVE TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            auto v = shuffledArray(100);
            for (auto i : v) { test.add(i); }
            logString += "added (key = i, value = 0) i = 1..100\n";
            test.remove(100);
            logString += "remove(100) called\n";
            auto size = test.size();
            logString += "SortedSequence.size() = " + std::to_string(size) +
                         " expected 99 : " + ((size == 99) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return size == 99;
        }

        static int testSizeConstruct(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE SIZE TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            auto size = test.size();
            logString += "SortedSequence.size() = " + std::to_string(size) +
                         " expected 0 : " + ((size == 0) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return size == 0;
        }

        static int testSizeAdd100(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE SIZE TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            auto v = shuffledArray(100);
            for (auto i : v) { test.add(i); }
            logString += "added 100 elements\n";
            auto size = test.size();
            logString += "SortedSequence.size() = " + std::to_string(size) +
                         " expected 100 : " + ((size == 100) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return size == 100;
        }

        static int testGetEmpty(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE GET TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "trying to get(0)\nexpected exception : ";
            bool exeptionAccepted = false;
            try {
                test.get(0);
            } catch (std::exception &) { exeptionAccepted = true; }
            logString += exeptionAccepted ? "OK" : "PROBLEM";
            stream << decoration(logString);
            return exeptionAccepted;
        }

        static int testGetExist(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE GET TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "added 5\n";
            test.add(5);
            auto got = test.get(0);
            logString += "SortedSequence.get(0) = " + std::to_string(got) +
                         " expected 5 : " + ((got == 5) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return got == 5;
        }

        static int testGetFirstEmpty(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE GETFIRST TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "trying to getFirst()\nexpected exception : ";
            bool exeptionAccepted = false;
            try {
                test.getFirst();
            } catch (std::exception &) { exeptionAccepted = true; }
            logString += exeptionAccepted ? "OK" : "PROBLEM";
            stream << decoration(logString);
            return exeptionAccepted;
        }

        static int testGetFirstExist(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE GETFIRST TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "added 5\n";
            test.add(5);
            auto got = test.getFirst();
            logString += "SortedSequence.getFirst() = " + std::to_string(got) +
                         " expected 5 : " + ((got == 5) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return got == 5;
        }


        static int testGetLastEmpty(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE GETLAST TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "trying to getLast()\nexpected exception : ";
            bool exeptionAccepted = false;
            try {
                test.getLast();
            } catch (std::exception &) { exeptionAccepted = true; }
            logString += exeptionAccepted ? "OK" : "PROBLEM";
            stream << decoration(logString);
            return exeptionAccepted;
        }

        static int testGetLastExist(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE GETLAST TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "added 5\n";
            test.add(5);
            auto got = test.getLast();
            logString += "SortedSequence.getLast() = " + std::to_string(got) +
                         " expected 5 : " + ((got == 5) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return got == 5;
        }


        static int testIndexOfNotExist(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE INDEXOF TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "trying to indexOf(0)\nexpected exception : ";
            bool exceptionAccepted = false;
            try {
                test.indexOf(0);
            } catch (std::exception &) { exceptionAccepted = true; }
            logString += exceptionAccepted ? "OK" : "PROBLEM";
            stream << decoration(logString);
            return exceptionAccepted;
        }

        static int testIndexOfExist(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE INDEXOF TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "added 5, 4, 8, 15, -7\n";
            test.add(5);
            test.add(4);
            test.add(8);
            test.add(15);
            test.add(-7);
            auto index = test.indexOf(5);
            logString += "SortedSequence.indexOf(5) = " + std::to_string(index) +
                         " expected 2 : " + ((index == 2) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return index == 2;
        }

        static int testIsEmptyEmpty(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE ISEMPTY TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            auto empty = test.isEmpty();
            logString += "SortedSequence.isEmpty() = " + std::to_string(empty) +
                         " expected true : " + ((empty) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return empty;
        }

        static int testIsEmptyNotEmpty(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE ISEMPTY TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "added 5\n";
            test.add(5);
            auto empty = test.isEmpty();
            logString += "SortedSequence.isEmpty() = " + std::to_string(empty) +
                         " expected false : " + ((!empty) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return !empty;
        }

        static int testGetSubsequenceInvalidIndices(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE GETSUBSEQUENCE TEST 1\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "trying to getSubsequence(10, 5)\nexpected exception : ";
            bool exeptionAccepted = false;
            try {
                test.getSubsequence(10,5);
            } catch (std::exception &) { exeptionAccepted = true; }
            logString += exeptionAccepted ? "OK" : "PROBLEM";
            stream << decoration(logString);
            return exeptionAccepted;
        }

        static int testGetSubsequenceValidIndices(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE GETSUBSEQUENCE TEST 2\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "added 5, 4, 8, 15, -7\n";
            test.add(5);
            test.add(4);
            test.add(8);
            test.add(15);
            test.add(-7);
            logString += "getSubsequence(2,3) called\n";
            auto subseq = test.getSubsequence(2,3);
            auto got = subseq -> get(0);
            logString += "Subsequence.get(0) = " + std::to_string(got) +
                         " expected 5 : " + ((got == 5) ? "OK" : "PROBLEM");
            delete subseq;
            stream << decoration(logString);
            return got == 5;
        }

        static int testCopy(std::ostream& stream) {
            std::string logString = std::string{"SORTED SEQUENCE COPY TEST\n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            logString += "added 5, 4, 8, 15, -7\n";
            test.add(5);
            test.add(4);
            test.add(8);
            test.add(15);
            test.add(-7);
            logString += "copy() called\n";
            auto cseq = test.copy();
            auto got = cseq -> get(0);
            logString += "Subsequence.get(0) = " + std::to_string(got) +
                         " expected -7 : " + ((got == -7) ? "OK" : "PROBLEM");
            delete cseq;
            stream << decoration(logString);
            return got == -7;
        }


        static int testClear(std::ostream &stream) {
            std::string logString = std::string{"SORTED SEQUENCE CLEAR TEST \n"}
                                    + "new SortedSequence constructed\n";
            SortedSequence<int>&& test = Sequence();
            auto v = shuffledArray(100);
            for (auto i : v) { test.add(i); }
            logString += "added 100 elements\n";
            test.clear();
            logString += "clear() called\n";
            auto size = test.size();
            logString += "SortedSequence.size() = " + std::to_string(size) +
                         " expected 0 : " + ((size == 0) ? "OK" : "PROBLEM");
            stream << decoration(logString);
            return size == 0;
        }

        static void testScript(std::ostream& stream, std::string name) {
            stream << "#######################\n" + name + " SORTED SEQUENCE TEST\n";
            int testsPassed = 0;
            testsPassed += testSizeConstruct(stream);
            testsPassed += testSizeAdd100(stream);
            testsPassed += testIsEmptyEmpty(stream);
            testsPassed += testIsEmptyNotEmpty(stream);
            testsPassed += testIndexOfNotExist(stream);
            testsPassed += testIndexOfExist(stream);
            testsPassed += testGetEmpty(stream);
            testsPassed += testGetExist(stream);
            testsPassed += testGetFirstEmpty(stream);
            testsPassed += testGetFirstExist(stream);
            testsPassed += testGetLastEmpty(stream);
            testsPassed += testGetLastExist(stream);
            testsPassed += testCopy(stream);
            testsPassed += testGetSubsequenceInvalidIndices(stream);
            testsPassed += testGetSubsequenceValidIndices(stream);
            testsPassed += testClear(stream);
            testsPassed += stressTestInsert(stream, 1000, 5);
            testsPassed += testRemoveNotExist(stream);
            testsPassed += testRemoveExist(stream);
            stream << testsPassed << "/19 : OK\n#######################";
        }
    };
}

#endif //LAB2_SORTEDSEQUENCETEST_HPP

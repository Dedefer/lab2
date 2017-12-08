//
// Created by Данила Фомин on 29.11.2017.
//

#ifndef LAB2_HASHTABLE_HPP
#define LAB2_HASHTABLE_HPP

#include "../Dictionary.hpp"

namespace lab2::dictionary {

    template <class KeyType, class ElementType>
    class HashTable : public Dictionary<KeyType, ElementType> {

    private:

        // types
        using NodePair = std::pair<KeyType, ElementType>;

        using ConstKeyRef = const KeyType&;

        using ConstElementRef = const ElementType&;

        struct CollisionList {


            struct Node {

                Node(ConstKeyRef key, ConstElementRef element, Node* nextNode = nullptr)
                        : field{key, element}, next{nextNode} {}

                Node(NodePair&& pair, Node* nextNode = nullptr) noexcept
                        : field{pair}, next{nextNode} {}

                NodePair field;

                Node* next;
            };

            Node* _head;

            // constructors

            CollisionList() noexcept : _head{nullptr}, _size{0} {}

            CollisionList(const CollisionList& obj) : _head{obj._head}, _size{obj._size} {
                if(obj._head) {
                    _head = new Node{*obj._head};
                    for(auto nodePtr = _head; nodePtr -> next; nodePtr = nodePtr -> next) {
                        nodePtr -> next = new Node{*(nodePtr -> next)};
                    }
                }
            }

            CollisionList(CollisionList&& obj) noexcept
            : _head{obj._head}, _size{obj._size} {
                obj._head = nullptr;
                obj._size = 0;
            }

            // operators =

            CollisionList& operator= (const CollisionList& rhs) {
                if (this != &rhs) {
                    clear();
                    _head = nullptr;
                    _size = rhs._size;
                    if(rhs._head) {
                        _head = new Node{*rhs._head};
                        for(auto nodePtr = _head; nodePtr -> next; nodePtr = nodePtr -> next) {
                            nodePtr -> next = new Node{*(nodePtr -> next)};
                        }
                    }
                }
                return *this;
            }

            CollisionList& operator= (CollisionList&& rhs) {
                if (this != &rhs) {
                    clear();
                    _head = rhs._head;
                    _size = rhs._size;
                    rhs._head = nullptr;
                    rhs._size = 0;
                }
                return *this;
            }

            // destructor

            ~CollisionList() { clear(); }

            // methods

            void clear() noexcept {
                for (auto nodePtr = _head; nodePtr;) {
                    auto tempPtr = nodePtr;
                    nodePtr = nodePtr -> next;
                    delete tempPtr;
                }
                _head = nullptr;
                _size = 0;
            }

            bool remove(ConstKeyRef key, ComparatorType<KeyType> equalComparator) noexcept {
                if (_head && equalComparator(_head -> field.first, key)) {
                    auto tempPtr = _head;
                    _head = _head -> next;
                    delete tempPtr;
                    --_size;
                    return true;
                }
                for (auto nodePtr = _head; nodePtr; nodePtr = nodePtr -> next) {
                    if (nodePtr -> next && equalComparator(nodePtr -> next -> field.first, key)) {
                        auto tempPtr = nodePtr -> next;
                        nodePtr -> next = nodePtr -> next -> next;
                        delete tempPtr;
                        --_size;
                        return true;
                    }
                }
                return false;
            }

            bool addOrModify(ConstKeyRef key, ConstElementRef element, ComparatorType<KeyType> equalComparator) {
                for (auto nodePtr = _head; nodePtr; nodePtr = nodePtr -> next) {
                    if (equalComparator(nodePtr -> field.first, key)) {
                        nodePtr -> field.second = element;
                        return false;
                    }
                }
                _head = new Node{key, element, _head};
                ++_size;
                return true;
            }

            void add(NodePair&& pair) {
                _head = new Node{std::move(pair), _head};
                ++_size;
            }

            size_t size() const noexcept { return _size; }

            ElementType find(ConstKeyRef key, ComparatorType<KeyType> equalComparator) {
                for (auto nodePtr = _head; nodePtr; nodePtr = nodePtr -> next) {
                    if (equalComparator(nodePtr -> field.first, key)) {
                        return nodePtr -> field.second;
                    }
                }
                throw std::out_of_range{"element with this key is missing"};
            }

            bool check(ConstKeyRef key, ComparatorType<KeyType> equalComparator) {
                for (auto nodePtr = _head; nodePtr; nodePtr = nodePtr -> next) {
                    if (equalComparator(nodePtr -> field.first, key)) {
                        return true;
                    }
                }
                return false;
            }

            void map(MapperType<ElementType> mapper) {
                for (auto nodePtr = _head; nodePtr; nodePtr = nodePtr -> next) {
                    mapper(nodePtr -> field.second);
                }
            }

        private:
            size_t _size;

        };

        // utility

        void _rebuild() {
            NodePair tempBuffer[_countOfElements];
            size_t tempIndex = 0;
            for (size_t i = 0; i < _size; ++i) {
                for (auto nodePtr = _slots[i]._head; nodePtr;
                     nodePtr = nodePtr -> next) {
                    tempBuffer[tempIndex++] = std::move(nodePtr -> field);
                }
            }

            tempIndex = 0;
            delete[] _slots;
            _size = _size << 1;
            _slots = new CollisionList[_size];

            for (; tempIndex < _countOfElements; ++tempIndex) {
                _slots[_getHash(tempBuffer[tempIndex].first)].add(std::move(tempBuffer[tempIndex]));
            }
        }

        size_t _getHash(ConstKeyRef key) const noexcept {
            return _hasher(key) & (_size - 1);
        }

        // fields

        ComparatorType<KeyType> _equalComparator;

        HasherType<KeyType> _hasher;

        size_t _size;

        size_t _countOfElements;

        CollisionList* _slots;

    public:


        // constructors

        HashTable(HasherType<KeyType> hasher = std::hash<KeyType>(),
                  ComparatorType<KeyType> equalComparator = equal<KeyType>())
                : _size{128}, _hasher{hasher}, _equalComparator{equalComparator},
                _slots{new CollisionList[_size]} {}

        HashTable(const HashTable<KeyType, ElementType>& obj) : _size{obj._size}, _hasher{obj._hasher},
                                                                _equalComparator{obj._equalComparator},
                                                                _countOfElements{0},
                                                                _slots{new CollisionList[obj._size]} {
            for (size_t i = 0; i < _size; ++i) {
                _slots[i] = obj._slots[i];
                ++_countOfElements;
            }
        }

        HashTable(HashTable<KeyType, ElementType>&& obj) noexcept : _size{obj._size}, _hasher{obj._hasher},
                                                                    _equalComparator{obj._equalComparator},
                                                                    _countOfElements{obj._countOfElements},
                                                                    _slots{obj._slots} {
            obj._hasher = std::hash<KeyType>();
            obj._equalComparator = equal<KeyType>();
            obj._countOfElements = 0;
            obj._size = 101;
            obj._slots = nullptr;
        }

        // operators =

        HashTable<KeyType, ElementType>& operator= (const HashTable<KeyType, ElementType>& rhs) {
            if (this != &rhs) {
                auto tempSlots = new CollisionList[_size];
                delete [] _slots;
                _slots = tempSlots;
                _size = rhs._size;
                _hasher = rhs._hasher;
                _equalComparator = rhs._equalComparator;
                _countOfElements = 0;
                for (size_t i = 0; i < _size; ++i) {
                    _slots[i] = rhs._slots[i];
                    ++_countOfElements;
                }

            }
            return *this;
        }


        HashTable<KeyType, ElementType>& operator= (HashTable<KeyType, ElementType>&& rhs) noexcept {
            if (this != &rhs) {
                delete [] _slots;
                _slots = rhs._slots;
                _size = rhs._size;
                _countOfElements = rhs._countOfElements;
                _equalComparator = rhs._equalComparator;
                _hasher = rhs._hasher;

                rhs._slots = nullptr;
                rhs._size = 101;
                rhs._countOfElements = 0;
                rhs._equalComparator = equal<KeyType>();
                rhs._hasher = std::hash<KeyType>();
            }
            return *this;
        }

        // destructor

        ~HashTable() override { delete [] _slots; }

        // overrided methods

        size_t size() const noexcept override { return _countOfElements; }

        size_t capacity() const noexcept override { return _size * 3; }

        ElementType get(ConstKeyRef key) const override {
            if (_slots) {
                auto index = _getHash(key);
                return _slots[index].find(key, _equalComparator);
            }
            throw std::out_of_range{"element with this key is missing"};
        }

        ElementType operator[](ConstKeyRef key) const override { return get(key); }

        bool check(ConstKeyRef key) const noexcept override {
            if (_slots) {
                auto index = _getHash(key);
                return _slots[index].check(key, _equalComparator);
            }
            return false;
        }

        void add(ConstKeyRef key, ConstElementRef element) override {
            if (!_slots) { _slots = new CollisionList[_size]; }
            auto index = _getHash(key);
            if (_slots[index].addOrModify(key, element, _equalComparator)) { ++_countOfElements; }
            if ((_slots[index].size() > 3)
                && ((_size << 1) < std::numeric_limits<size_t>::max())) {
                _rebuild();
            }
        }

        void remove(ConstKeyRef key) noexcept override {
            if (_slots) {
                auto index = _getHash(key);
                if (_slots[index].remove(key, _equalComparator)) { --_countOfElements; }
            }
        }

        void clear() noexcept override {
            if (_slots) {
                for (size_t i = 0; i < _size; ++i) { _slots[i].clear(); }
                _countOfElements = 0;
            }
        }

        void map(MapperType<ElementType> mapper) override {
            for (size_t i = 0; i < _size; ++i) { _slots[i].map(mapper); }
        }

    };

}
#endif //LAB2_HASHTABLE_HPP

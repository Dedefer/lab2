//
// Created by Данила Фомин on 30.11.2017.
//

#ifndef LAB2_RBTREE_HPP
#define LAB2_RBTREE_HPP

#include "../Dictionary.hpp"

namespace lab2::dictionary {

    template <class KeyType, class ElementType>
    class RBTree : public Dictionary<KeyType, ElementType> {

    private:

        // types

        using ConstKeyRef = const KeyType&;

        using ConstElementRef = const ElementType&;

        struct RBNode {

            using NodePair = std::pair<KeyType, ElementType>;

            RBNode *lSon, *rSon, *parent;

            bool isRed;

            NodePair field;

            RBNode(ConstKeyRef key, ConstElementRef element,
                   RBNode* parent = nullptr, RBNode* lSon = nullptr,
                   RBNode* rSon = nullptr)
                    : field{key, element}, lSon{lSon}, rSon{rSon},
                      parent{parent}, isRed{true} {}

            void changeColor() { isRed = !isRed; }

        };

        // utility methods

        bool _nodeIsRightSon(RBNode* node) const { return node -> parent -> rSon == node; }

        RBNode* _uncle(RBNode* node) const {
            auto pNode = node -> parent;
            if (_nodeIsRightSon(pNode)) { return pNode -> parent -> lSon; }
            return pNode -> parent -> rSon;
        }

        void _lRotate(RBNode* node) {
            auto tempNode = node -> rSon;
            tempNode -> parent = node -> parent;
            if (tempNode -> parent) {
                if (_nodeIsRightSon(node)) {
                    tempNode -> parent -> rSon = tempNode;
                } else {
                    tempNode -> parent -> lSon = tempNode;
                }
            } else { _root = tempNode; }
            node -> rSon = tempNode -> lSon;
            if (tempNode -> lSon) { tempNode -> lSon -> parent = node; }
            node -> parent = tempNode;
            tempNode -> lSon = node;
        }

        void _rRotate(RBNode* node) {
            auto tempNode = node -> lSon;
            tempNode -> parent = node -> parent;
            if (tempNode -> parent) {
                if (_nodeIsRightSon(node)) {
                    tempNode -> parent -> rSon = tempNode;
                } else {
                    tempNode -> parent -> lSon = tempNode;
                }
            } else { _root = tempNode; }
            node -> lSon = tempNode -> rSon;
            if (tempNode -> rSon) { tempNode -> rSon -> parent = node; }
            node -> parent = tempNode;
            tempNode -> rSon = node;
        }

        // add utility

        void _addBalance(RBNode* node) {
            auto pNode = node -> parent;
            if (pNode) {
                if (pNode -> isRed) {
                    auto uncle = _uncle(node);
                    if (uncle && uncle -> isRed) {
                        _addBalance_case2(pNode, uncle);
                        return;
                    }
                    bool parentIsRightSon = _nodeIsRightSon(pNode);
                    bool nodeIsRightSon = _nodeIsRightSon(node);
                    bool nodeAndUncleIsCooriented = parentIsRightSon != nodeIsRightSon;
                    if (nodeAndUncleIsCooriented) {
                        _addBalance_case3(node, pNode, pNode -> parent, nodeIsRightSon);
                    } else {
                        _addBalance_case4(node, pNode, pNode -> parent, parentIsRightSon);
                    }
                }
            } else { _addBalance_case1(node); }
        }

            void _addBalance_case1(RBNode* node) { node -> changeColor(); }

            void _addBalance_case2(RBNode* pNode, RBNode* uNode) {
                pNode -> changeColor();
                pNode -> parent -> changeColor();
                uNode -> changeColor();
                _addBalance(pNode -> parent);
            }

            void _addBalance_case3(RBNode* node, RBNode* pNode,
                                   RBNode* gpNode, bool nodeIsRightSon) {
                if (nodeIsRightSon) { _lRotate(pNode); }
                else { _rRotate(pNode); }
                _addBalance_case4(pNode, node, gpNode, !nodeIsRightSon);
            }

            void _addBalance_case4(RBNode* node, RBNode* pNode,
                                   RBNode* gpNode, bool pNodeIsRightSon) {
                if (pNodeIsRightSon) { _lRotate(gpNode); }
                else {_rRotate(gpNode); }
                gpNode -> changeColor();
                pNode -> changeColor();
            }

        // clear utility

        void _recurciveClear(RBNode* node) noexcept {
            if (node) {
                _recurciveClear(node -> lSon);
                _recurciveClear(node -> rSon);
                delete node;
            }
        }

        // copy utility

        void _recurciveCopy(RBNode* node) {
            if (node) {
                if (node -> lSon) {
                    node -> lSon = new RBNode{*(node -> lSon)};
                    node -> lSon -> parent = node;
                }
                if (node -> rSon) {
                    node -> rSon = new RBNode{*(node -> rSon)};
                    node -> rSon -> parent = node;
                }
                _recurciveCopy(node -> lSon);
                _recurciveCopy(node -> rSon);
            }
        }

        // remove utility

        // fields

        ComparatorType<KeyType> _equalComparator;

        ComparatorType<KeyType> _relationComparator;

        RBNode* _root;

        size_t _size;

    public:


        // constructors

        RBTree(ComparatorType<KeyType> relationComparator = less<KeyType>(),
               ComparatorType<KeyType> equalComparator = equal<KeyType>()) noexcept
                : _relationComparator{relationComparator}, _equalComparator{equalComparator},
                  _root{nullptr}, _size{0} {}

        RBTree(const RBTree& obj) : _size{obj._size}, _relationComparator{obj._relationComparator},
                                    _equalComparator{obj._equalComparator}, _root{nullptr} {
            if (obj._root) {
                _root = new RBNode{*obj._root};
                _recurciveCopy(_root);
            }
        }

        RBTree(RBTree&& obj) noexcept : _relationComparator{obj._relationComparator},
                                        _equalComparator{obj._equalComparator},
                                        _size{obj._size}, _root{obj._root} {
            obj._equalComparator = equal<KeyType>();
            obj._relationComparator = less<KeyType>();
            obj._root = nullptr;
            obj._size = 0;
        }

        // operators =

        RBTree<KeyType, ElementType>& operator= (const RBTree<KeyType, ElementType>& rhs) {
            if (this != &rhs) {
                _relationComparator = rhs._relationComparator;
                _equalComparator = rhs._equalComparator;
                _size = rhs._size;
                if (rhs._root) {
                    _root = new RBNode{*rhs._root};
                    _recurciveCopy(_root);
                } else { _root = nullptr; }
            }
            return *this;
        }

        RBTree<KeyType, ElementType>& operator= (RBTree<KeyType, ElementType>&& rhs) noexcept {
            if (this != &rhs) {
                _relationComparator = rhs._relationComparator;
                _equalComparator = rhs._equalComparator;
                _size = rhs._size;
                _root = rhs._root;

                rhs._relationComparator = less<KeyType>();
                rhs._equalComparator = equal<KeyType>();
                rhs._size = 0;
                rhs._root = nullptr;
            }
            return *this;
        }

        // destructor

        ~RBTree() override { _recurciveClear(_root); }

        // overrided methods

        size_t getCount() const noexcept override { return _size; }

        ElementType get(ConstKeyRef key) const override {
            for (auto node = _root; node;) {
                if (_equalComparator(node -> field.first, key)) { return node -> field.second; }
                if (_relationComparator(key, node -> field.first)) { node = node -> lSon; }
                else { node = node -> rSon; }
            }
            throw std::out_of_range{"element with this key is missing"};
        }

        ElementType operator[](ConstKeyRef key) const override { return get(key); }

        bool check(ConstKeyRef key) const noexcept override {
            for (auto node = _root; node;) {
                if (_equalComparator(node -> field.first, key)) { return true; }
                if (_relationComparator(key, node -> field.first)) { node = node -> lSon; }
                else { node = node -> rSon; }
            }
            return false;
        }

        void add(ConstKeyRef key, ConstElementRef element) override {
            RBNode* addedNode = nullptr;
            if (_root) {
                for (auto node = _root; node;) {
                    if (_equalComparator(node -> field.first, key)) {
                        node -> field.second = element;
                        return;
                    }
                    if (_relationComparator(key, node -> field.first)) {
                        if (node -> lSon) {
                            node = node -> lSon;
                            continue;
                        } else {
                            addedNode = new RBNode{key, element, node};
                            node -> lSon = addedNode;
                            break;
                        }
                    } else {
                        if (node -> rSon) {
                            node = node -> rSon;
                            continue;
                        } else {
                            addedNode = new RBNode{key, element, node};
                            node -> rSon = addedNode;
                            break;
                        }
                    }
                }
            } else {
                _root = new RBNode{key, element};
                addedNode = _root;
            }
            if (addedNode) {
                ++_size;
                _addBalance(addedNode);
            }
        }

        void remove(ConstKeyRef key) noexcept override {

        }

        void clear() noexcept override {
            _recurciveClear(_root);
            _root = nullptr;
            _size = 0;
        }

    };
}


#endif //LAB2_RBTREE_HPP

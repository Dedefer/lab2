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

        /*
        // debug utility

        bool _isValid(RBNode* node, int depth) {
            if (node) {
                bool lSonValid = true;
                bool rSonValid = true;
                if (node->isRed) {
                    lSonValid = node -> lSon ? !(node -> lSon -> isRed) : true;
                    rSonValid = node -> rSon ? !(node -> rSon -> isRed) : true;
                } else { ++depth; }
                return lSonValid && rSonValid && _isValid(node -> lSon, depth) && _isValid(node -> rSon, depth);
            } else {std::cout << depth << " ";}
            return true;
        }
        */

        // utility methods

        bool _nodeIsRightSon(RBNode* node) const noexcept { return node -> parent -> rSon == node; }

        RBNode* _uncle(RBNode* node) const {
            auto pNode = node -> parent;
            if (_nodeIsRightSon(pNode)) { return pNode -> parent -> lSon; }
            return pNode -> parent -> rSon;
        }

        void _lRotate(RBNode* node) noexcept {
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

        void _rRotate(RBNode* node) noexcept {
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

            void _addBalance_case1(RBNode* node) { node -> changeColor(); } // node is root

            void _addBalance_case2(RBNode* pNode, RBNode* uNode) { // node's parent and uncle are red
                pNode -> changeColor();
                pNode -> parent -> changeColor();
                uNode -> changeColor();
                _addBalance(pNode -> parent);
            }

            void _addBalance_case3(RBNode* node, RBNode* pNode,
                                   RBNode* gpNode, bool nodeIsRightSon) { // node and uncle have similar orientation and uncle is black
                if (nodeIsRightSon) { _lRotate(pNode); }
                else { _rRotate(pNode); }
                _addBalance_case4(pNode, node, gpNode, !nodeIsRightSon);
            }

            void _addBalance_case4(RBNode* node, RBNode* pNode,
                                   RBNode* gpNode, bool pNodeIsRightSon) { // node and uncle have different orientation and uncle is black
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

        // remove utility (in all cases node has zero or one sons)

        RBNode* _onlySon(RBNode* node) noexcept {
            return node -> rSon ? node -> rSon : node -> lSon;
        }

        RBNode* _sibling(RBNode* node) noexcept {
            return _nodeIsRightSon(node) ? node -> parent -> lSon
                                         : node -> parent -> rSon;
        }

        void _removeBalance(RBNode* node) noexcept {
            if (node -> isRed) { _removeBalance_case2(node); }
            else if (node -> lSon || node -> rSon) { _removeBalance_case1(node); }
            else {
                _removeBalance_case3(node);
                if (node -> parent) {
                    if (_nodeIsRightSon(node)) { node -> parent -> rSon = nullptr; }
                    else { node -> parent -> lSon = nullptr; }
                } else { _root = nullptr; }
            }
        }

            void _removeBalance_case1(RBNode* node) noexcept { // node is black, son is red
                auto son = _onlySon(node);
                son -> parent = node -> parent;
                if (son -> parent) {
                    if (_nodeIsRightSon(node)) { son -> parent -> rSon = son; }
                    else { son -> parent -> lSon = son; }
                } else { _root = son; }
                son -> changeColor();
            }

            void _removeBalance_case2(RBNode* node) noexcept { // node is red, son is black
                auto son = _onlySon(node);
                if (son) { son -> parent = node -> parent; }
                if (_nodeIsRightSon(node)) { node -> parent -> rSon = son; }
                else { node -> parent -> lSon = son; }
            }

            // after you call it from _removeBalance you must manualy replace node with nullptr
            void _removeBalance_case3(RBNode* node) noexcept { // node is black, sons are null (or recursive)
                auto pNode = node -> parent;
                if (pNode) {
                    auto sNode = _sibling(node);
                    if (sNode -> isRed) {
                        _removeBalance_case3_subcase1(node); // pNode -> isRed : false
                        return;
                    } else if (pNode -> isRed) {
                        _removeBalance_case3_subcase345(node);
                    } else {
                        if ((sNode -> lSon && sNode -> lSon -> isRed) || // sibling has red son?
                            (sNode -> rSon && sNode -> rSon -> isRed)) {
                            if (_nodeIsRightSon(node) ? sNode -> lSon : sNode -> rSon) { // node and sibling's son have different orientation?
                                _removeBalance_case3_subcase5(node);
                            } else { _removeBalance_case3_subcase4(node); }
                        } else { _removeBalance_case3_subcase2(node); }
                    }
                }
            }

                void _removeBalance_case3_subcase1(RBNode* node) noexcept { // sibling is red (parent is black)
                    auto sNode = _sibling(node);
                    auto pNode = node -> parent;
                    if (_nodeIsRightSon(sNode)) { _lRotate(pNode); }
                    else { _rRotate(pNode); }
                    pNode -> changeColor();
                    sNode -> changeColor();
                    _removeBalance_case3_subcase345(node);
                }

                void _removeBalance_case3_subcase2(RBNode* node) noexcept { // parent and sibling are black, sibling's sons are null
                    _sibling(node) -> changeColor();
                    _removeBalance_case3(node -> parent);
                }

                void _removeBalance_case3_subcase345(RBNode* node) noexcept { // agregated for ..._subcase1
                    auto sNode = _sibling(node);
                    if ((sNode -> lSon && sNode -> lSon -> isRed) || // sibling has red son?
                        (sNode -> rSon && sNode -> rSon -> isRed)) {
                        if (_nodeIsRightSon(node) ? sNode -> lSon : sNode -> rSon) { // node and sibling's son have different orientation?
                            _removeBalance_case3_subcase5(node);
                        } else { _removeBalance_case3_subcase4(node); }
                    } else { _removeBalance_case3_subcase3(node); }
                }

                void _removeBalance_case3_subcase3(RBNode* node) noexcept { // parent is red, sibling is black, sibling's sons are null
                    node -> parent -> changeColor();
                    _sibling(node) -> changeColor();
                }

                void _removeBalance_case3_subcase4(RBNode* node) noexcept { // sibling is black, subsibling is red and
                                                                                // have similar orientation to node
                    auto sNode = _sibling(node);
                    auto ssNode = _nodeIsRightSon(sNode) ? sNode -> lSon
                                                         : sNode -> rSon;
                    if (_nodeIsRightSon(ssNode)) { _lRotate(sNode); }
                    else { _rRotate(sNode); }
                    sNode -> changeColor();
                    ssNode -> changeColor();
                    _removeBalance_case3_subcase5(node);
                }

                void _removeBalance_case3_subcase5(RBNode* node) noexcept {  // sibling is black, subsibling is red and
                                                                                 // have different orientation to node
                    auto pNode = node -> parent;
                    auto sNode = _sibling(node);
                    auto ssNode = _nodeIsRightSon(sNode) ? sNode -> rSon
                                                         : sNode -> lSon;
                    if (_nodeIsRightSon(ssNode)) { _lRotate(pNode); }
                    else { _rRotate(pNode); }
                    ssNode -> changeColor();
                    sNode -> isRed = pNode -> isRed;
                    pNode -> isRed = false;
                }

        // map utility

        void  _recursiveMap(MapperType<ElementType> mapper, RBNode* node) {
            if (node) {
                mapper(node -> field.second);
                _recursiveMap(mapper, node -> lSon);
                _recursiveMap(mapper, node -> rSon);
            }
        }




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

        size_t size() const noexcept override { return _size; }

        size_t capacity() const noexcept override { return  _size; }

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
            bool keyFound = false;
            auto node = _root;
            while (node) {
                if (_equalComparator(key, node -> field.first)) {
                    keyFound = true;
                    break;
                }
                if (_relationComparator(key, node -> field.first)) { node = node -> lSon; }
                else { node = node -> rSon; }
            }
            if (keyFound) {
                if (node -> lSon && node -> rSon) {
                    auto tempNode = node -> rSon;
                    for (;tempNode -> lSon;
                         tempNode = tempNode -> lSon);
                    node -> field = std::move(tempNode -> field);
                    _removeBalance(tempNode);
                    delete tempNode;
                } else {
                    _removeBalance(node);
                    delete node;
                }
                --_size;
            }
        }

        void clear() noexcept override {
            _recurciveClear(_root);
            _root = nullptr;
            _size = 0;
        }

        void map(MapperType<ElementType> mapper) override {
            _recursiveMap(mapper, _root);
        }

    };
}


#endif //LAB2_RBTREE_HPP

#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key, Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix( AVLNode<Key, Value>* p, AVLNode<Key,Value>* n);
    void removeFix( AVLNode<Key, Value>* n, int8_t diff);
    void rotateLeft( AVLNode<Key, Value>* node);
    void rotateRight( AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft( AVLNode<Key, Value>* x) {
  if(x == nullptr) return;
  AVLNode<Key,Value>* y = x->getRight();
  AVLNode<Key,Value>* p = x->getParent();
  AVLNode<Key,Value>* b = y->getLeft();

  if(p != nullptr){
    if(p->getLeft() == x){
      p->setLeft(y);
    }
    else{
      p->setRight(y);
    }
  }
  else {
    this->root_ = y;
  }
  y->setParent(p);
  y->setLeft(x);
  x->setParent(y);
  x->setRight(b);
  if(b != nullptr){
    b->setParent(x);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight( AVLNode<Key, Value>* y) {
  if(y == nullptr) return;
  AVLNode<Key,Value>* x = y->getLeft();
  AVLNode<Key,Value>* p = y->getParent();
  AVLNode<Key,Value>* b = x->getRight();

  if(p != nullptr)
  {
    if(p->getLeft() == y){
      p->setLeft(x);
    }
    else{
      p->setRight(x);
    }
  }
  else{
    this->root_ = x;
  }

  y->setParent(x);
  y->setLeft(b);
  x->setParent(p); 
  x->setRight(y);
  if(b != nullptr){
    b->setParent(y);
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key, Value>* parent, AVLNode<Key,Value>* node) {
    if(parent == nullptr || parent->getParent() == nullptr) return;
    AVLNode<Key,Value>* grandParent = parent->getParent();

    //if parent is left child of the gparent, else = parent is right child of gparent
    if(parent == grandParent->getLeft()) {
        grandParent->updateBalance(-1);

        if(grandParent->getBalance() == 0) return;
        else if(grandParent->getBalance() == -1){ insertFix(grandParent, parent); }
        else if(grandParent->getBalance() == -2) {
            //if zig-zig
            if(parent->getBalance() == -1) {
                rotateRight(grandParent); 
                parent->setBalance(0); grandParent->setBalance(0);
            }
            else{
                rotateLeft(parent); rotateRight(grandParent);
                if(node->getBalance() == -1) {
                    parent->setBalance(0); grandParent->setBalance(1); node->setBalance(0);
                }
                else if(node->getBalance() == 0) {
                    parent->setBalance(0); grandParent->setBalance(0); node->setBalance(0);
                }
                else if(node->getBalance() == 1) {
                    parent->setBalance(-1); grandParent->setBalance(0); node->setBalance(0);
    }}}}
    else{
        grandParent->updateBalance(1);

        if(grandParent->getBalance() == 0) return;
        else if(grandParent->getBalance() == 1){ insertFix(grandParent, parent); }
        else if(grandParent->getBalance() == 2) {
            //if zig-zig
            if(parent->getBalance() == 1) {
                rotateLeft(grandParent); 
                parent->setBalance(0); grandParent->setBalance(0);
            }
            else{
                rotateRight(parent); rotateLeft(grandParent);
                if(node->getBalance() == 1) {
                    parent->setBalance(0); grandParent->setBalance(-1); node->setBalance(0);
                }
                else if(node->getBalance() == 0) {
                    parent->setBalance(0); grandParent->setBalance(0); node->setBalance(0);
                }
                else if(node->getBalance() == -1) {
                    parent->setBalance(1); grandParent->setBalance(0); node->setBalance(0);
    }}}}

}


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->empty()) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    }
    else if(this->internalFind(new_item.first) != nullptr) //key is already in tree, update value
    {
        this->internalFind(new_item.first)->setValue(new_item.second);
    }
    else
    {
        AVLNode<Key, Value>* parent = nullptr;
        AVLNode<Key, Value>* currentNode = static_cast<AVLNode<Key, Value>*>(this->root_);
        while( currentNode != nullptr )
        {
            parent = currentNode;
            if(new_item.first < currentNode->getKey()) {
                currentNode = currentNode->getLeft();
            }
            else{
                currentNode = currentNode->getRight();
            }
        }
        currentNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);

        bool leftNode = false;
        if(parent->getKey() > currentNode->getKey()) {
            leftNode = true;
        }


        if(leftNode) {
            parent->setLeft(currentNode); 
        }
        else {
            parent->setRight(currentNode);
        }

        if(parent->getBalance() == -1 || parent->getBalance() == 1) { parent->setBalance(0); }
        else if(parent->getBalance() == 0) {
            if(leftNode) {
                parent->setBalance(-1);
            }
            else {
                parent->setBalance(1);
            }
            insertFix(parent, currentNode);
        }

    }
}


template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n, int8_t diff) {
    if(n == nullptr) return;

    AVLNode<Key, Value>* p = n->getParent(); int8_t ndiff=0;
    if(p != nullptr) {
        if(n == p->getLeft()) {
            ndiff = 1;
        }
        else if(n == p->getRight()){
            ndiff = -1;
        }
    }

    if(diff == -1) {
        //case1
        if(n->getBalance() + diff == -2){
            AVLNode<Key, Value>* c = n->getLeft();
            //case 1a
            if(c == nullptr) return;
            if(c->getBalance() == -1) {
                rotateRight(n); n->setBalance(0); c->setBalance(0);
            }
            //case 1b
            else if(c->getBalance() == 0) {
                rotateRight(n); n->setBalance(-1); c->setBalance(1); removeFix(p, ndiff);
            }
            //case 1c
            else if(c->getBalance() == 1) {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c); rotateRight(n);
                if(g->getBalance() == 1){
                    n->setBalance(0); c->setBalance(-1); g->setBalance(0);
                }
                else if(g->getBalance() == 0){
                    n->setBalance(0); c->setBalance(0); g->setBalance(0);
                }
                else if(g->getBalance() == -1){
                    n->setBalance(1); c->setBalance(0); g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        //case 2
        else if(n->getBalance() + diff == -1) {
            n->setBalance(-1);
        }
        //case 3
        else if(n->getBalance() + diff == 0) {
            n->setBalance(0); removeFix(p, ndiff);
        }
    }
    else if(diff == 1) {
        //case1
        if(n->getBalance() + diff == 2){
            AVLNode<Key, Value>* c = n->getRight();
            //case 1a
            if(c->getBalance() == 1) {
                rotateLeft(n); n->setBalance(0); c->setBalance(0); removeFix(p, ndiff);
            }
            //case 1b
            else if(c->getBalance() == 0) {
                rotateLeft(n); n->setBalance(1); c->setBalance(-1);
            }
            //case 1c
            else if(c->getBalance() == -1) {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c); rotateLeft(n);
                    if(g->getBalance() == -1){
                        n->setBalance(0); c->setBalance(1); g->setBalance(0);
                    }
                    else if(g->getBalance() == 0){
                        n->setBalance(0); c->setBalance(0); g->setBalance(0);
                    }
                    else if(g->getBalance() == 1){
                        n->setBalance(-1); c->setBalance(0); g->setBalance(0);
                    }
                removeFix(p, ndiff);
            }
        }
        //case 2
        else if(n->getBalance() + diff == 1) {
            n->setBalance(1);
        }
        //case 3
        else if(n->getBalance() + diff == 0) {
            n->setBalance(0); removeFix(p, ndiff);
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(this->internalFind(key) == nullptr) return;
    AVLNode<Key, Value>* removeNode = static_cast<AVLNode<Key, Value>*>(this->internalFind(key)); 
    int8_t diff=0;

    if(removeNode->getLeft() != nullptr && removeNode->getRight() != nullptr) {
        AVLNode<Key, Value>* swap = static_cast<AVLNode<Key, Value>*>(this->predecessor(removeNode));
        nodeSwap(removeNode, swap);
    }

    AVLNode<Key, Value>* parent = removeNode->getParent();
    if(parent != nullptr){
        if(removeNode == parent->getLeft()) {
            diff = 1;
        }
        else if(removeNode == parent->getRight()) {
            diff = -1;
        }
    }

    if(removeNode->getLeft() == nullptr && removeNode->getRight() == nullptr) {
        if(removeNode == this->root_) {
            this->root_ = nullptr;
        }
        else if(parent->getLeft() == removeNode) {
            parent->setLeft(nullptr);
        }
        else if(parent->getRight() == removeNode) {
            parent->setRight(nullptr);
        }
    }
    //ONE CHILD CASE
    else if(removeNode->getLeft() == nullptr || removeNode->getRight() == nullptr ) {
        Node<Key, Value>* child;
        if(removeNode->getLeft() != nullptr){
            child = removeNode->getLeft();
        }
        else{
            child = removeNode->getRight();
        }

        if(removeNode == this->root_){
          this->root_ = child;
        }
        else if(removeNode == parent->getLeft()) {
            parent->setLeft(child);
        }
        else {
            parent->setRight(child);
        }

        if(child != nullptr) {
            child->setParent(parent);  
        }
    }

    delete removeNode;
    removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

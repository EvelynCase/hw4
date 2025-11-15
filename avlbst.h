#ifndef AVLBST_H
#define AVLBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor
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
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO 

    // 1. insert 

    // base case: if tree is empty 
    if(this->root_ == nullptr){
      // just add new node from root 
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr); // dynamically allocate a new node to insert 
      return; // done
    
    }

    AVLNode<Key, Value>* temp = this->root_; // start temp at the root 
    AVLNode<Key, Value>* tempParent = nullptr; // so that we can insert the node later  

    // A. walk the tree until find an empty location 
    while (temp != nullptr){
      
      // go left if value is less than node --> value here would be the key bc that's how BST stores nodes 
      if(new_item.first < temp->getKey()){
        tempParent = temp; // update parent
        temp = temp->getLeft();
      }
      // right if greater than node
      else if(new_item.first > temp->getKey()){
        tempParent = temp; // update parent
        temp = temp->getRight();
      }
      // else the value is equal --> key is already in the tree so overwrite !!
      else{
        temp->setValue(new_item.second); // set new value
        return; // overwritten so now done 
      }
    }

    // B. insert the new node
    AVLNode<Key, Value>* nodeToInsert = new AVLNode<Key, Value>(new_item.first, new_item.second, tempParent); // create a new node 

    // if item's key is < parent's key
    if(new_item.first < tempParent->getKey()){  
      tempParent->setLeft(nodeToInsert); // go left 
      parent->updateBalance(-1); // update the balance with the added node of left side
    }
    // its greater than so go right
    else{
      tempParent->setRight(nodeToInsert); // go right 
      parent->updateBalance(1); // update the balance with the added node of right side
    }

    // 2. Balance the tree 
    balanceTree(temp); 


    

    return;
}

// helper function to balance the tree: 
template<class Key, class Value>
void AVLTree<Key, Value>:: balanceTree(AVLNode<Key, Value>* temp){
    
    AVLNode<Key, Value>* tempParent = temp->getParent(); // tempParent stores the parent of temp 

    // base case: 
    if(tempParent == nullptr){
        return; // temp is the root so its all good 
    }

    // get grand parent
    AVLNode<Key, Value>* tempGrandParent = tempParent->getParent(); // tempGrandParent stores the grandparent of temp 

    if(tempParent->getBalance() == 0){ // if parent is balanced --> all good
        return; 
    }
    // else tree is not balanced and need to balance it now: 
    if(temp == tempParent->getRight()){ // temp is the right kid 
        // fill 
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

    // case in which the tree is empty 
  if(root_ == nullptr){
    return; // just return 
  }

  Node<Key, Value>* temp = root_; // start temp at the root 
  Node<Key, Value>* tempParent = nullptr; // so that we can remove nodes with kids 

  // 1. walk the tree to find the value to remove --> traverse down 
  while(temp != nullptr){ // while ptr is not at end 

    if(key < temp->getKey()){
      tempParent = temp; // update parent 
      temp = temp->getLeft(); // key < so go left 
    }
    else if(key > temp->getKey()){
      tempParent = temp; // update parent 
      temp = temp->getRight(); // key < so go left 
    }
    else {
      // else the key matches !! 
      break; // and temp points to the node to remove 
    }
  }

  if(temp == nullptr){
    return; // key was not found :(
  }

  // 2. remove the node 
  
  // case if node to remove has zero kids: remove leaf 
  if(temp->getLeft() == nullptr && temp->getRight() == nullptr){
    
    // case if root is the node to delete 
    if(tempParent == nullptr){
      root_ = nullptr;
    }
    // case if left kid
    else if(tempParent->getLeft() == temp){ // temp is left kid 
      tempParent->setLeft(nullptr); 
    }
    // case if right kid 
    else if(tempParent->getRight() == temp){ // temp is right kid 
      tempParent->setRight(nullptr); 
    }

    delete temp; // delete 
    return;
  } 

  // A. case if nodeToRemove has 2 kids: swap the value with its predecessor -> remove from it's new location 
  if(temp->getLeft() != nullptr && temp->getRight() != nullptr) {
    Node<Key, Value>* tempPredecessor = predecessor(temp); // to store the predecessor 

    /* if(tempPredecessor != nullptr){ // while tempPredecessor exisits
      
      Key predecessorKey = tempPredecessor->getKey(); // store original key before the swap 

      nodeSwap(tempPredecessor, temp); // swap the keys !! 
      remove(predecessorKey); // remove from new location 

      // swap the items rather than the nodes: 
      
    } */

    if(tempPredecessor == nullptr)
      return;

    nodeSwap(tempPredecessor, temp);
    tempParent = temp->getParent();
  }

  // B. case if node to remove has one kid: connect parent and grandkid and delete temp 
  Node<Key, Value>* tempKid = nullptr; // tempKid stores the kid of temp 

  if(temp->getLeft() == nullptr && temp->getRight() != nullptr){ // if temp has a right kid 
    tempKid = temp->getRight(); // rightKid stores the right kid
  }
  else if(temp->getLeft() != nullptr && temp->getRight() == nullptr){ // if temp has a left kid
    tempKid = temp->getLeft(); // leftKid stores the right kid
  }
  else{
    tempKid = nullptr; // no kids so leaf and return null
  }
 
  if(tempKid != nullptr){
    tempKid->setParent(tempParent); // connect the parent
  }

  if(tempParent == nullptr){ // the root is the node to deletee
    root_ = tempKid; 
  }
  else if(tempParent->getLeft() == temp){ // temp is left kid 
    tempParent->setLeft(tempKid); // connect parent
  }
  else { // temp is right kid 
    tempParent->setRight(tempKid); // connect parent
  }


  delete temp; // delete  
  return;
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

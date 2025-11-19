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

    // helper function to balance the tree after an insertion or deletion
    void balanceTree(AVLNode<Key, Value>* tempParent, int rol);
    // void balanceTreeForInsert(AVLNode<Key, Value>* tempParent, int rol);
    // void balanceTreeForRemove(AVLNode<Key, Value>* tempParent, int rol);
    void rightRotate(AVLNode<Key, Value>* temp, AVLNode<Key, Value>* tempParent);
    void leftRotate(AVLNode<Key, Value>* temp, AVLNode<Key, Value>* tempParent);

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

    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_); // start temp at the root 
    AVLNode<Key, Value>* tempParent = nullptr; // so that we can insert the node later  

    // A. walk the tree until find an empty location 
    while (temp != nullptr){
      tempParent = temp; // to not fall off the end of the tree and have temp be nullptr after 
      // go left if value is less than node --> value here would be the key bc that's how BST stores nodes 
      if(new_item.first < temp->getKey()){
        // tempParent = temp; // update parent
        temp = temp->getLeft();
      }
      // right if greater than node
      else if(new_item.first > temp->getKey()){
        // tempParent = temp; // update parent
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
      // tempParent->updateBalance(1); // update the balance with the added node of left side
    
      // 2. Balance the tree 
      balanceTree(tempParent, 1); 
    }
    // its greater than so go right
    else{
      tempParent->setRight(nodeToInsert); // go right 
      // tempParent->updateBalance(-1); // update the balance with the added node of right side
    
      // 2. Balance the tree 
      balanceTree(tempParent, -1); 
      // balanceTreeForInsert(tempParent, -1);
    }

    return;
}

// helper function to balance the tree: 
template<class Key, class Value>
void AVLTree<Key, Value>:: balanceTree(AVLNode<Key, Value>* tempParent, int rol){ // rol stores the number that node added to the tree like +1 is added as right kid or -1 is left 
    
  //start at tempParent and walk upward to balance along that path
  AVLNode<Key, Value>* treeIterator = tempParent; // start at tempParent to iterate the tree 

  // base case: 
  if(treeIterator == nullptr) { //  || tempParent->getBalance() == 0) {
    return; // temp is the root so its all good 
  }

  // 1. find balance factor 
  treeIterator->updateBalance(rol); // set the balance of the iterator!
  int8_t balanceFactor = treeIterator->getBalance(); 

  // if tree IS balanced
  if(balanceFactor == 0){
    return; // done !
  }
  // if tree is NOT balanced 
  if(balanceFactor < -1){ // heavy on right kids 

    AVLNode<Key, Value>* rightKid = treeIterator->getRight(); // get the right kid
    
    if(rightKid != nullptr && rightKid->getBalance() > 0){
      // case for RL 
      // rightRotate(rightKid, treeIterator);
      rightRotate(nullptr, rightKid);
    }
    // else {
      // case for RR 
      leftRotate(nullptr, treeIterator);
      return; // the end so return 
    // }
  }
  else if(balanceFactor > 1){ // heavy on left kids 
    
    AVLNode<Key, Value>* leftKid = treeIterator->getLeft(); // get the right kid
    
    if(leftKid != nullptr && leftKid->getBalance() < 0){
      // case for LR 
      // leftRotate(leftKid, treeIterator);
      leftRotate(nullptr, leftKid);
    }
    // else {
      // case for LL 
      rightRotate(nullptr, treeIterator);
      return; // the end so return 
    // }
  }

  // get grand parent
  AVLNode<Key, Value>* tempGrandParent = treeIterator->getParent(); // tempGrandParent stores the grandparent of temp 
  
  if(tempGrandParent == nullptr){
    return; // we reached the root ! done !
  }

  // get the next rol 
  int r; 
  if(treeIterator == tempGrandParent->getLeft()){
    r = 1; // then left kids heavier
  }
  else {
    r = -1; // then right kids heavier 
  }
  
  // recursive call: 
  balanceTree(tempGrandParent, r); // move up the tree along the path tyo the root

  return; // balanceFactor is zero and good so return !
}





/*
template<class Key, class Value>
void AVLTree<Key, Value>:: balanceTreeForInsert(AVLNode<Key, Value>* tempParent, int rol){ // rol stores the number that node added to the tree like +1 is added as right kid or -1 is left 
    
  //start at tempParent and walk upward to balance along that path
  AVLNode<Key, Value>* treeIterator = tempParent; // start at tempParent to iterate the tree 

  // base case: 
  if(treeIterator == nullptr) { //  || tempParent->getBalance() == 0) {
    return; // temp is the root so its all good 
  }

  // 1. find balance factor 
  treeIterator->updateBalance(rol); // set the balance of the iterator!
  int8_t balanceFactor = treeIterator->getBalance(); 

  // if tree IS balanced
  if(balanceFactor == 0){
    return; // done !
  }
  // if tree is NOT balanced 
  if(balanceFactor < -1){ // heavy on right kids 

    AVLNode<Key, Value>* rightKid = treeIterator->getRight(); // get the right kid
    
    if(rightKid != nullptr && rightKid->getBalance() > 0){
      // case for RL 
      // rightRotate(rightKid, treeIterator);
      rightRotate(nullptr, rightKid);
    }
    // else {
      // case for RR 
      leftRotate(nullptr, treeIterator);
      return; // the end so return 
    // }
  }
  else if(balanceFactor > 1){ // heavy on left kids 
    
    AVLNode<Key, Value>* leftKid = treeIterator->getLeft(); // get the right kid
    
    if(leftKid != nullptr && leftKid->getBalance() < 0){
      // case for LR 
      // leftRotate(leftKid, treeIterator);
      leftRotate(nullptr, leftKid);
    }
    // else {
      // case for LL 
      rightRotate(nullptr, treeIterator);
      return; // the end so return 
    // }
  }

  // get grand parent
  AVLNode<Key, Value>* tempGrandParent = treeIterator->getParent(); // tempGrandParent stores the grandparent of temp 
  
  if(tempGrandParent == nullptr){
    return; // we reached the root ! done !
  }

  // get the next rol 
  int r; 
  if(treeIterator == tempGrandParent->getLeft()){
    r = 1; // then left kids heavier
  }
  else {
    r = -1; // then right kids heavier 
  }
  
  // recursive call: 
  balanceTreeForInsert(tempGrandParent, r); // move up the tree along the path tyo the root

  return; // balanceFactor is zero and good so return !
}



template<class Key, class Value>
void AVLTree<Key, Value>:: balanceTreeForRemove(AVLNode<Key, Value>* tempParent, int rol){ // rol stores the number that node added to the tree like +1 is added as right kid or -1 is left 
    
  //start at tempParent and walk upward to balance along that path
  AVLNode<Key, Value>* treeIterator = tempParent; // start at tempParent to iterate the tree 

  // base case: 
  if(treeIterator == nullptr) { //  || tempParent->getBalance() == 0) {
    return; // temp is the root so its all good 
  }

  // 1. find balance factor 
  treeIterator->updateBalance(rol); // set the balance of the iterator!
  int8_t balanceFactor = treeIterator->getBalance(); 

  // if tree IS balanced
  //if(balanceFactor == 0){
  //  return; // done !
  //}

  if(balanceFactor == 1 || balanceFactor == -1){
    return; // the subtree height didn't change !! so just retrunr
  }

  // if tree is NOT balanced 
  if(balanceFactor < -1){ // heavy on right kids 

    AVLNode<Key, Value>* rightKid = treeIterator->getRight(); // get the right kid
    
    // find balanceFactor for rightKid: 
    int8_t balanceFactorOfRightKid;
    if(rightKid != nullptr){
      balanceFactorOfRightKid = rightKid->getBalance();
    }
    else{
      balanceFactorOfRightKid = 0; // 
    }

    if(rightKid != nullptr && balanceFactorOfRightKid > 0){
      // case for RL 
      // rightRotate(rightKid, treeIterator);
      rightRotate(nullptr, rightKid);
    }
    // else {
      // case for RR 
      leftRotate(nullptr, treeIterator);

      if(balanceFactorOfRightKid != 0){
        return; // height of this subtree did not decrease 
      }
    // }
  }
  else if(balanceFactor > 1){ // heavy on left kids 
    
    AVLNode<Key, Value>* leftKid = treeIterator->getLeft(); // get the right kid
    
    // find balanceFactor for leftKid: 
    int8_t balanceFactorOfLeftKid;
    if(leftKid != nullptr){
      balanceFactorOfLeftKid = leftKid->getBalance();
    }
    else{
      balanceFactorOfLeftKid = 0; // 
    }

    if(leftKid != nullptr && leftKid->getBalance() < 0){
      // case for LR 
      // leftRotate(leftKid, treeIterator);
      leftRotate(nullptr, leftKid);
    }
      // else 
      // case for LL 
      rightRotate(nullptr, treeIterator);

      if(balanceFactorOfLeftKid != 0){
        return;
      }
      // return; // the end so return 
    // }
  }
  // then balanceFactor will be good so continue on 

  // get grand parent
  AVLNode<Key, Value>* tempGrandParent = treeIterator->getParent(); // tempGrandParent stores the grandparent of temp 
  
  if(tempGrandParent == nullptr){
    return; // we reached the root ! done !
  }

  // get the next rol 
  int r; 
  if(treeIterator == tempGrandParent->getLeft()){
    r = -1; // then left kids heavier
  }
  else {
    r = 1; // then right kids heavier 
  }
  
  // recursive call: 
  balanceTreeForRemove(tempGrandParent, r); // move up the tree along the path tyo the root

  return; // balanceFactor is zero and good so return !
}

*/





// helper function to rotate nodes right 
template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* temp, AVLNode<Key, Value>* tempParent){
  
  // follow x, y, z rotation 
  AVLNode<Key, Value>* z = tempParent; // z is the parent 
  
  // base case: 
  if(tempParent == nullptr){
    return; // temp is the root so all good ! 
  }

  AVLNode<Key, Value>* y = z->getLeft(); // y is left kid 

  // base case: 
  if(y == nullptr){
    return; // all good 
  }

  // figure out LL, RR, LR, RL: 
  AVLNode<Key, Value>* zigzag = y->getRight(); // zig zag 
  AVLNode<Key, Value>* tempGrandParent = z->getParent();

  // rotate x, y, z: 
  y->setRight(z);
  z->setParent(y); 
  z->setLeft(zigzag);

  if(zigzag != nullptr){
    zigzag->setParent(z); 
  }

  y->setParent(tempGrandParent); 
  if(tempGrandParent == nullptr){ // at root! 
    this->root_ = y; // set the new root 
  }
  else if(tempGrandParent->getLeft() == z){
    tempGrandParent->setLeft(y); // swap with y 
  }
  else{
    tempGrandParent->setRight(y);
  }

  // reset the balance: 
  z->setBalance(0);
  y->setBalance(0);

  return;
} 





// helper function to rotate nodes left 
template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* temp, AVLNode<Key, Value>* tempParent){

  // follow x, y, z rotation 
  AVLNode<Key, Value>* z = tempParent; // z is the parent 
  
  // base case: 
  if(tempParent == nullptr){
    return; // temp is the root so all good ! 
  }

  AVLNode<Key, Value>* y = z->getRight(); // y is left kid 

  // base case: 
  if(y == nullptr){
    return; // all good 
  }

  // figure out LL, RR, LR, RL: 
  AVLNode<Key, Value>* zagzig = y->getLeft(); // zig zag 
  AVLNode<Key, Value>* tempGrandParent = z->getParent();

  // rotate x, y, z: 
  y->setLeft(z);
  z->setParent(y); 
  z->setRight(zagzig);

  if(zagzig != nullptr){
    zagzig->setParent(z); 
  }

  y->setParent(tempGrandParent); 
  if(tempGrandParent == nullptr){ // at root! 
    this->root_ = y; // set the new root 
  }
  else if(tempGrandParent->getLeft() == z){
    tempGrandParent->setLeft(y); // swap with y 
  }
  else{
    tempGrandParent->setRight(y);
  }

  // reset the balance: 
  z->setBalance(0);
  y->setBalance(0);

  return;
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
  if(this->root_ == nullptr){
    return; // just return 
  }

  Node<Key, Value>* temp = this->root_; // start temp at the root 
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

    // keep track of height: 
    bool removedFromLeft = false; // flag for which side removed on 
    bool removedFromRight = false; 
    
    // case if root is the node to delete 
    if(tempParent == nullptr){
      this->root_ = nullptr;
    }
    // case if left kid
    else if(tempParent->getLeft() == temp){ // temp is left kid 
      tempParent->setLeft(nullptr); 
      removedFromLeft = true;
    }
    // case if right kid 
    else if(tempParent->getRight() == temp){ // temp is right kid 
      tempParent->setRight(nullptr); 
      removedFromRight = true; 
    }

    // Rebalance !!! 
    if(tempParent != nullptr){
      AVLNode<Key, Value>* avlPtrParent = static_cast<AVLNode<Key, Value>*>(tempParent);
      int rol = 0; // create rol for balanceTree 

      if(removedFromLeft){
        rol = -1; // right kids heavier 
      }
      if(removedFromRight){
        rol = 1; // left kids heavier 
      }

      if(rol != 0){
        balanceTree(avlPtrParent, rol);
        // balanceTreeForRemove(avlPtrParent, rol);
      }
    }

    delete temp; // delete 
    return;
  } 

  // A. case if nodeToRemove has 2 kids: swap the value with its predecessor -> remove from it's new location 
  if(temp->getLeft() != nullptr && temp->getRight() != nullptr) {
    Node<Key, Value>* tempPredecessor = BinarySearchTree<Key, Value>::predecessor(temp); // to store the predecessor 

    if(tempPredecessor == nullptr)
      return;

    this->nodeSwap(static_cast<AVLNode<Key, Value>*>(tempPredecessor), static_cast<AVLNode<Key, Value>*>(temp));
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

  // reset the flags 
  bool removedFromLeft = false; // flag for which side removed on 
  bool removedFromRight = false;

  if(tempParent == nullptr){ // the root is the node to deletee
    this->root_ = tempKid; 
  }
  else if(tempParent->getLeft() == temp){ // temp is left kid 
    tempParent->setLeft(tempKid); // connect parent
    removedFromLeft = true; 
  }
  else { // temp is right kid 
    tempParent->setRight(tempKid); // connect parent
    removedFromRight = true; 
  }

  // Rebalance !!! 
  if(tempParent != nullptr){
    AVLNode<Key, Value>* avlPtrParent = static_cast<AVLNode<Key, Value>*>(tempParent);
    int rol = 0; // create rol for balanceTree 

    if(removedFromLeft){
      rol = -1; // right kids heavier 
    }
    if(removedFromRight){
      rol = 1; // left kids heavier 
    }

    if(rol != 0){
      balanceTree(avlPtrParent, rol);
    }
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

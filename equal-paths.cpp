#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int helperFunc(Node* n); // helper function to recurse down the tree until finds a leaf --> returns level in number

bool equalPaths(Node * root)
{
  // Add your code below

  // base case: 
  if( root == nullptr){
      return true; // equal distance 
  }

  // if found a leaf, return true
  if(root->left == nullptr && root->right == nullptr){
      return true;
  }

  // if the depth of the left and right sides are not equal, return false!
  if(helperFunc(root->right) != helperFunc(root->left)){ // get depth of right and left and compare
      return false;
  }
  else{ // else they are equal so continue  through the tree 
    
    // recurse down the tree
    bool rightSide = equalPaths(root->right); // rightSide returns true if rightSide's nodes are all equal depth
    bool leftSide = equalPaths(root->left); // lefttSide returns true if lefttSide's nodes are all equal depth
  
    // if both sides true, then equal depth and good to return true 
    if(rightSide == true && leftSide == true){
      return true;
    }
    else{
      return false;
    }
  }

  return false; // if not caught before

}

// recurses down the tree until leaf and returns the depth as an int 
int helperFunc(Node* n){
  
  // base case 
  if(n == nullptr){
    return 0; // depth is zero
  }

  // if no kids
  if(n->left == nullptr && n->right == nullptr){
    return 0; // + 0 because no kids 
  }

  // if has only a right kid 
  if(n->left == nullptr){
    return helperFunc(n->right) + 1; // + 1 because 1 kid 
  }

  // if has only a left kid 
  if(n->right == nullptr){
    return helperFunc(n->left) + 1; // + 1 because 1 kid 
  }

  // if has both kids 
  if(n->left != nullptr && n->right != nullptr){
    return helperFunc(n->left) + 1; // + 1 because a kid and just choose a path to go down, doesn't matter
  }

}

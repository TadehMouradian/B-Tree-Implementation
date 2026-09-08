#include "BTreeNode.h"

//Precondition: N/A
//Postcondition: A BTreeNode object is created with default information
BTreeNode::BTreeNode() : key{0}, child{nullptr}, count(0), isLeaf(true) {}

//Precondition: N/A
//Postcondition: A BTreeNode object is created with given information
BTreeNode::BTreeNode(int* key, BTreeNode** child, int count, bool isLeaf){
    for(int i = 0; i < MAX; i++){
        this->key[i] = key[i];
        this->child[i] = child[i];
    }
    this->child[MAX] = child[MAX];
    this->count = count;
    this->isLeaf = isLeaf;
}

//Precondition: N/A
//Postcondition: The array of keys is returned as a pointer to the array
int* BTreeNode::getKeys(){
    return key;
}

//Precondition: N/A
//Postcondition: The array of children is returned as a pointer to the array
BTreeNode** BTreeNode::getChildren(){
    return child;
}

//Precondition: N/A
//Postcondition: The number of keys is returned
int BTreeNode::getCount() const{
    return count;
}

//Precondition: N/A
//Postcondition: Whether or not this node is a leaf is returned
bool BTreeNode::getIsLeaf() const{
    return isLeaf;
}

//Precondition: A pointer to an int array of size MAX is entered
//Postcondition: Sets the keys of this node to the given keys
void BTreeNode::setKeys(int* key){
    for(int i = 0; i < MAX; i++){
        this->key[i] = key[i];
    }
}

//Precondition: A pointer to a BTreeNode* array of size MAX+1 is entered
//Postcondition: Sets the children of this node to the given children
void BTreeNode::setChildren(BTreeNode** child){
    for(int i = 0; i <= MAX; i++){
        this->child[i] = child[i];
    }
}

//Precondition: N/A
//Postcondition: Sets the count of the number of keys to the given count
void BTreeNode::setCount(int count){
    this->count = count;
}

//Precondition: N/A
//Postcondition: Sets the boolean value of whether or not this node is a leaf to the given boolean value
void BTreeNode::setIsLeaf(bool isLeaf){
    this->isLeaf = isLeaf;
}
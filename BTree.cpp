#include "BTree.h"
#include <iostream>
#include <stack>
using namespace std;

//Precondition: N/A
//Postcondition: A BTree object is created with default information
BTree::BTree() : root(nullptr) {}

//Precondition: N/A
//Postcondition: A BTree object is created as a copy of another BTree object
BTree::BTree(const BTree& other){
    stack<BTreeNode*> stk;
    stack<BTreeNode*> otherStk;

    if(other.root){ // Traverse through the other tree and copy the information to this tree
        root = new BTreeNode();
        stk.push(root);
        otherStk.push(other.root);

        while(!stk.empty()){
            BTreeNode* cur = stk.top();
            BTreeNode* otherCur = otherStk.top();
            stk.pop();
            otherStk.pop();

            int curKeys[MAX] = {0};
            int* otherKeys = otherCur->getKeys();
            for(int i = 0; i < otherCur->getCount(); i++){
                curKeys[i] = otherKeys[i];
            }

            BTreeNode* curChildren[MAX+1] = {nullptr};
            BTreeNode** otherChildren = otherCur->getChildren();
            for(int i = 0; i <= otherCur->getCount(); i++){
                if(otherChildren[i] != nullptr){
                    curChildren[i] = new BTreeNode();
                    stk.push(curChildren[i]);
                    otherStk.push(otherChildren[i]);
                }
            }

            cur->setKeys(curKeys);
            cur->setChildren(curChildren);
            cur->setCount(otherCur->getCount());
            cur->setIsLeaf(otherCur->getIsLeaf());
        }
    }
}

//Precondition: N/A
//Postcondition: This BTree object is deleted
BTree::~BTree(){
    stack<BTreeNode*> stk;
    if(root){
        stk.push(root);
    }
    while(!stk.empty()){ // Traverse through this tree and delete the nodes
        BTreeNode* temp = stk.top();
        stk.pop();

        for(int i = 0; i <= temp->getCount(); i++){
            if(temp->getChildren()[i] != nullptr){
                stk.push(temp->getChildren()[i]);
            }
        }
        
        delete temp;
    }
}

//Precondition: N/A
//Postcondition: data is inserted into the BTree
void BTree::insert(int data){
    if(!root){ // if nothing is in the tree, make a new root with data as its only key
        root = new BTreeNode();
        int key[MAX] = {0};
        key[0] = data;
        root->setKeys(key);
        root->setCount(1);
    }
    else{
        stack<BTreeNode*> stk;
        stk.push(root);
        bool inserted = false;

        while(!inserted){ // loop until data is inserted
            if(!stk.top()->getIsLeaf()){ // if the top of the stack is not a leaf node, keep searching through tree for the correct leaf node to insert data, while pushing the nodes we traversed into the stack
                BTreeNode* temp = stk.top();
                for(int i = 0; i <= temp->getCount(); i++){
                    if(i == temp->getCount() || temp->getKeys()[i] > data){
                        stk.push(temp->getChildren()[i]);
                        break;
                    }
                }
            }
            else{ // we have hit the leaf node to insert data
                inserted = true;
                //leftNode and rightNode are the two children associated with data, initially they are nullptrs, but if splitting occurs they hold the two children made from the split
                BTreeNode* leftNode = nullptr, *rightNode = nullptr;
                
                while(!stk.empty() && stk.top()->getCount() == MAX){ // loop runs if inserting into this node will cause a split
                    // an array of keys and an array of children is created that are 1 element larger than normal to fit all the keys and children
                    int allKeys[MAX+1] = {0};
                    BTreeNode* allChildren[MAX+2] = {nullptr};
                    BTreeNode* cur = stk.top();
                    stk.pop();
                    bool added = false;

                    int j = 0, k = 0, z = 0;
                    for(int i = 0; i < MAX+1; i++){ // all the keys and children of the node + data and leftNode and rightNode are sorted into the two arrays
                        if(!added && (cur->getKeys()[j] > data || i == MAX)){
                            allKeys[i] = data;
                            allChildren[z] = leftNode;
                            allChildren[z+1] = rightNode;
                            delete cur->getChildren()[k]; // this node was the one that was split, so it is deleted, since its data is already split into leftNode and rightNode with the median value being data
                            z++;
                            k++;
                            added = true;
                        }
                        else{
                            allKeys[i] = cur->getKeys()[j];
                            j++;
                            allChildren[z] = cur->getChildren()[k];
                            k++;
                        }
                        z++;
                    }
                    if(k != cur->getCount()+1){
                        allChildren[MAX+1] = cur->getChildren()[k];
                    }

                    // data becomes the median value of the array of keys, and leftNode and rightNode become the two children created from the split
                    data = allKeys[(MAX+1)/2];
                    int leftKeys[MAX] = {0};
                    int rightKeys[MAX] = {0};
                    BTreeNode* leftChildren[MAX+1] = {nullptr};
                    BTreeNode* rightChildren[MAX+1] = {nullptr};
                    for(int i = 0; i <= (MAX+1)/2; i++){
                        if(i < (MAX+1)/2){
                            leftKeys[i] = allKeys[i];
                        }
                        leftChildren[i] = allChildren[i];
                    }
                    for(int i = 0; i <= (MAX+1)/2; i++){
                        if(i < (MAX+1)/2){
                            rightKeys[i] = allKeys[i + 1 + (MAX+1)/2];
                        }
                        rightChildren[i] = allChildren[i + 1 + (MAX+1)/2];
                    }

                    leftNode = new BTreeNode(leftKeys, leftChildren, MAX/2, leftChildren[0] == nullptr);
                    rightNode = new BTreeNode(rightKeys, rightChildren, MAX/2, rightChildren[0] == nullptr);

                    // the loop will keep trying to insert the median value into the parent node, while keeping track of the children formed by the split by using leftNode and rightNode
                    // the loop will end when the stack is empty (new root must be made and tree gains a level), or a node is found with space to insert the median value
                }

                if(stk.empty()){ // once splitting has been taken care of and no node remains in the stack, then a new root needs to be created with data as its only key, and leftNode and rightNode as its children
                    int rootKeys[MAX] = {0};
                    rootKeys[0] = data;
                    BTreeNode* rootChildren[MAX+1] = {nullptr};
                    rootChildren[0] = leftNode;
                    rootChildren[1] = rightNode;
                    root = new BTreeNode(rootKeys, rootChildren, 1, false);
                }
                else{ // if there is a node in the stack that isn't full, insert data, leftNode, and rightNode into this node
                    BTreeNode* temp = stk.top();
                    int newKeys[MAX] = {0};
                    BTreeNode* newChildren[MAX+1] = {nullptr};

                    bool added = false;
                    int j = 0, k = 0, z = 0;
                    for(int i = 0; i <= temp->getCount(); i++){
                        if(!added && (temp->getKeys()[j] > data || i == temp->getCount())){
                            newKeys[i] = data;
                            newChildren[z] = leftNode;
                            newChildren[z+1] = rightNode;
                            delete temp->getChildren()[k]; // this node was the one that was split, so it is deleted, since its data is already split into leftNode and rightNode with the median value being data
                            z++;
                            k++;
                            added = true;
                        }
                        else{
                            newKeys[i] = temp->getKeys()[j];
                            j++;
                            newChildren[z] = temp->getChildren()[k];
                            k++;
                        }
                        z++;
                    }

                    temp->setKeys(newKeys);
                    temp->setChildren(newChildren);
                    temp->setCount(temp->getCount() + 1);
                }
            }
        }
    }
}

//Precondition: data is an element in this tree
//Postcondition: One instance of data is removed from the BTree
void BTree::remove(int data){
    if(!root){ // if nothing is in the tree, then there is nothing to remove
        return;
    }

    stack<BTreeNode*> stk;
    stk.push(root);
    bool deleted = false;

    while(!deleted){ // loop until data is deleted
        if(!stk.top()->getIsLeaf()){
            BTreeNode* temp = stk.top();
            for(int i = 0; i <= temp->getCount(); i++){ // if the top of the stack is not a leaf node, keep searching through tree for the correct leaf node where data should be, while pushing the nodes we traversed into the stack
                if(i < temp->getCount() && temp->getKeys()[i] == data){ // if data is not in a leaf node, replace data with its immediate predecessor and keep traversing down to the leaf node
                    BTreeNode* temp2 = temp->getChildren()[i];
                    while(!temp2->getIsLeaf()){
                        temp2 = temp2->getChildren()[temp2->getCount()];
                    }
                    int* keys = temp->getKeys();
                    keys[i] = temp2->getKeys()[temp2->getCount()-1];
                    temp->setKeys(keys);
                    keys = temp2->getKeys();
                    keys[temp2->getCount()-1] = data;
                    temp2->setKeys(keys);
                    stk.push(temp->getChildren()[i]);
                    break;
                }
                else if(i == temp->getCount() || temp->getKeys()[i] > data){
                    stk.push(temp->getChildren()[i]);
                    break;
                }
            }
        }
        else{ // found the leaf node with data
            deleted = true;
            bool removed = false;
            BTreeNode* cur = stk.top();
            int* keys = cur->getKeys();

            // remove data from the node
            for(int i = 0; i < cur->getCount() - 1; i++){
                if(keys[i] == data || removed){
                    keys[i] = keys[i+1];
                    removed = true;
                }
            }
            cur->setKeys(keys);
            cur->setCount(cur->getCount()-1);

            BTreeNode* parent;
            while(!stk.empty() && ((stk.size() > 1 && stk.top()->getCount() < MAX/2) || (stk.top()->getCount() < 1))){ // loop continues while the number of keys in the current node is less than the minimum
                cur = stk.top();
                stk.pop();

                if(stk.empty()){ // if the current node has no parent, then the current node is the root and it has 0 keys, so the tree losses a level
                    BTreeNode* temp = root;
                    root = root->getChildren()[0];
                    delete temp;
                    break;
                }
                else{
                    parent = stk.top();
                }

                // get the index location of the current node in relation to its parent's children
                int idx;
                for(int i = 0; i <= parent->getCount(); i++){
                    if(parent->getChildren()[i] == cur){
                        idx = i;
                        break;
                    }
                }

                if(idx != 0 && parent->getChildren()[idx-1]->getCount() > MAX/2){ // if the node has a left sibling that it can borrow from
                    BTreeNode* swap = parent->getChildren()[idx-1]; // borrow from left sibling

                    int* curKeys = cur->getKeys();
                    BTreeNode** curChildren = cur->getChildren();
                    int* swapKeys = swap->getKeys();
                    BTreeNode** swapChildren = swap->getChildren();
                    int* parentKeys = parent->getKeys();
                    BTreeNode** parentChildren = parent->getChildren();

                    for(int i = cur->getCount()+1; i > 0; i--){ // make space at the start of the current node to insert the parent key
                        if(i <= cur->getCount()){
                            curKeys[i] = curKeys[i-1];
                        }
                        curChildren[i] = curChildren[i-1];
                    }

                    // insert the parent key and swap the parent key with the left sibling's rightmost key
                    curKeys[0] = parentKeys[idx-1];
                    curChildren[0] = swapChildren[swap->getCount()]; // rightmost child of the left sibling becomes the leftmost child of the current node
                    parentKeys[idx-1] = swapKeys[swap->getCount()-1];
                    swapChildren[swap->getCount()] = nullptr;

                    cur->setKeys(curKeys);
                    cur->setChildren(curChildren);
                    swap->setKeys(swapKeys);
                    swap->setChildren(swapChildren);
                    parent->setKeys(parentKeys);
                    parent->setChildren(parentChildren);

                    cur->setCount(cur->getCount()+1);
                    swap->setCount(swap->getCount()-1);
                }
                else if(idx != parent->getCount() && parent->getChildren()[idx+1]->getCount() > MAX/2){ // if the node has a right sibling that it can borrow from
                    // logic is the same as the previous case but the node to swap from is the right sibling instead of the left sibling
                    BTreeNode* swap = parent->getChildren()[idx+1];

                    int* curKeys = cur->getKeys();
                    BTreeNode** curChildren = cur->getChildren();
                    int* swapKeys = swap->getKeys();
                    BTreeNode** swapChildren = swap->getChildren();
                    int* parentKeys = parent->getKeys();
                    BTreeNode** parentChildren = parent->getChildren();

                    curKeys[cur->getCount()] = parentKeys[idx];
                    curChildren[cur->getCount()+1] = swapChildren[0];
                    parentKeys[idx] = swapKeys[0];
                    swapChildren[0] = nullptr;

                    for(int i = 0; i < swap->getCount(); i++){
                        if(i < swap->getCount()-1){
                            swapKeys[i] = swapKeys[i+1];
                        }
                        swapChildren[i] = swapChildren[i+1];
                    }
                    swapChildren[swap->getCount()] = nullptr;

                    cur->setKeys(curKeys);
                    cur->setChildren(curChildren);
                    swap->setKeys(swapKeys);
                    swap->setChildren(swapChildren);
                    parent->setKeys(parentKeys);
                    parent->setChildren(parentChildren);

                    cur->setCount(cur->getCount()+1);
                    swap->setCount(swap->getCount()-1);
                }
                else{ // cannot borrow from a sibling, so we must merge instead
                    BTreeNode* merge;
                    if(idx == 0){ // if the current node is the leftmost child, merge it with its right sibling
                        merge = parent->getChildren()[idx+1]; // merge with right sibling

                        int* curKeys = cur->getKeys();
                        BTreeNode** curChildren = cur->getChildren();
                        int* mergeKeys = merge->getKeys();
                        BTreeNode** mergeChildren = merge->getChildren();
                        int* parentKeys = parent->getKeys();
                        BTreeNode** parentChildren = parent->getChildren();

                        curKeys[1] = parentKeys[idx]; // the parent key associated with this node and the two keys from the right sibling are added to this node
                        for(int i = 0; i < merge->getCount(); i++){
                            curKeys[i+2] = mergeKeys[i];
                        }

                        for(int i = 0; i < merge->getCount()+1; i++){ // the children of the right sibling are also added to this node
                            curChildren[i+2] = mergeChildren[i];
                        }
         
                        delete parentChildren[idx+1]; // all data has been moved from the right sibling, so delete the node
                        parentChildren[idx+1] = nullptr;

                        // remove gaps in the parent node from bringing down a key
                        for(int i = idx; i < parent->getCount() - 1; i++){
                            parentKeys[i] = parentKeys[i+1];
                        }
                        for(int i = idx+1; i < parent->getCount(); i++){
                            parentChildren[i] = parentChildren[i+1];
                        }

                        parent->setKeys(parentKeys);
                        parent->setChildren(parentChildren);
                        cur->setKeys(curKeys);
                        cur->setChildren(curChildren);
                        parent->setCount(parent->getCount() - 1);
                        cur->setCount(MAX);
                    }
                    else{ // if the node is not the leftmost child, merge it with its left sibling
                        // logic is the same as the previous case, except the merge node is the left sibling instead of the right one
                        merge = parent->getChildren()[idx-1];

                        int* curKeys = cur->getKeys();
                        BTreeNode** curChildren = cur->getChildren();
                        int* mergeKeys = merge->getKeys();
                        BTreeNode** mergeChildren = merge->getChildren();
                        int* parentKeys = parent->getKeys();
                        BTreeNode** parentChildren = parent->getChildren();

                        curKeys[merge->getCount() + 1] = curKeys[0];
                        for(int i = 0; i < merge->getCount(); i++){
                            curKeys[i] = mergeKeys[i];
                        }
                        curKeys[merge->getCount()] = parentKeys[idx-1];

                        curChildren[merge->getCount() + 1] = cur->getChildren()[0];
                        curChildren[merge->getCount() + 2] = cur->getChildren()[1];
                        for(int i = 0; i < merge->getCount()+1; i++){
                            curChildren[i] = mergeChildren[i];
                        }
                        
                        delete parentChildren[idx-1];
                        parentChildren[idx-1] = nullptr;

                        for(int i = idx-1; i < parent->getCount(); i++){
                            if(i < parent->getCount() - 1){
                                parentKeys[i] = parentKeys[i+1];
                            }
                            parentChildren[i] = parentChildren[i+1];
                        }

                        parent->setKeys(parentKeys);
                        parent->setChildren(parentChildren);
                        cur->setKeys(curKeys);
                        cur->setChildren(curChildren);
                        parent->setCount(parent->getCount() - 1);
                        cur->setCount(MAX);
                    }
                }
                // the loop will keep going until the tree structure no longer has any issues, or until it reaches the root, and the root has no keys (tree losses a level)
            }
        }
    }
}

//Precondition: N/A
//Postcondition: This BTree object becomes a copy of another BTree object
BTree& BTree::operator=(const BTree& other){
    stack<BTreeNode*> stk;
    stack<BTreeNode*> otherStk;

    if(root){ // Traverse through this tree and delete the nodes
        stk.push(root);
    }
    while(!stk.empty()){
        BTreeNode* temp = stk.top();
        stk.pop();

        for(int i = 0; i <= temp->getCount(); i++){
            if(temp->getChildren()[i] != nullptr){
                stk.push(temp->getChildren()[i]);
            }
        }
        
        delete temp;
    }
    root = nullptr;

    if(other.root){ // Traverse through the other tree and copy the information to this tree
        root = new BTreeNode();
        stk.push(root);
        otherStk.push(other.root);

        while(!stk.empty()){
            BTreeNode* cur = stk.top();
            BTreeNode* otherCur = otherStk.top();
            stk.pop();
            otherStk.pop();

            int curKeys[MAX] = {0};
            int* otherKeys = otherCur->getKeys();
            for(int i = 0; i < otherCur->getCount(); i++){
                curKeys[i] = otherKeys[i];
            }

            BTreeNode* curChildren[MAX+1] = {nullptr};
            BTreeNode** otherChildren = otherCur->getChildren();
            for(int i = 0; i <= otherCur->getCount(); i++){
                if(otherChildren[i] != nullptr){
                    curChildren[i] = new BTreeNode();
                    stk.push(curChildren[i]);
                    otherStk.push(otherChildren[i]);
                }
            }
            
            cur->setKeys(curKeys);
            cur->setChildren(curChildren);
            cur->setCount(otherCur->getCount());
            cur->setIsLeaf(otherCur->getIsLeaf());
        }
    }

    return *this;
}

//Precondition: N/A
//Postcondition: The helper function is called, and this BTree is printed out in order
void BTree::printInorder(){
    printInorderHelper(root);
}

//Precondition: N/A
//Postcondition: This BTree is printed out in order
void BTree::printInorderHelper(BTreeNode* node){
    if(node != nullptr && node->getIsLeaf()){ // Base case where node is a leaf, so print out all the keys
        for(int i = 0; i < node->getCount(); i++){
            cout << node->getKeys()[i] << " ";
        }
    }
    else if(node != nullptr){ // If node isn't a nullptr, print the left child of the current key, then the current key
        for(int i = 0; i < node->getCount(); i++){
            printInorderHelper(node->getChildren()[i]);
            cout << "\n" << node->getKeys()[i] << "\n";
        }
        printInorderHelper(node->getChildren()[node->getCount()]); // print final, rightmost, child after all keys have been printed
    }
}
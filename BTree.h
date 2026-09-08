#ifndef BTREE_H
#define BTREE_H
#include "BTreeNode.h"
class BTree{
    private:
        BTreeNode* root;
        
        void printInorderHelper(BTreeNode*);
    public:
        BTree(); // Constructor
        BTree(const BTree&); // Copy Constructor
        ~BTree(); // Destructor

        void insert(int); // Member functions
        void remove(int);
        void printInorder();

        BTree& operator=(const BTree&); // Operator =
};
#endif
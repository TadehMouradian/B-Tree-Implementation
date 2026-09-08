#ifndef BTREENODE_H
#define BTREENODE_H
const int MAX = 4;

class BTreeNode{
    private:
        int key[MAX];
        BTreeNode* child[MAX+1];
        int count;
        bool isLeaf;
    public:
        BTreeNode();
        BTreeNode(int*, BTreeNode**, int, bool); // Constructors

        int* getKeys(); // Getters
        BTreeNode** getChildren();
        int getCount() const;
        bool getIsLeaf() const;

        void setKeys(int*); // Setters
        void setChildren(BTreeNode**);
        void setCount(int);
        void setIsLeaf(bool);
};
#endif
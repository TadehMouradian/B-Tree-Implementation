// B Tree Project
// Tadeh Mouradian 10728802
// 12/7/25
#include <iostream>
#include "BTreeNode.h"
#include "BTree.h"
using namespace std;

int main(){
    BTree tree = BTree();
    int key, numElements;
    char choice;
    cout << "Enter the number of elements: ";
    cin >> numElements;

    for(int i = 1; i <= numElements; i++){ // Insert elements into the tree
        cout << "Enter the element " << i << " : ";
        cin >> key;
        tree.insert(key);
    }

    cout << "\nConstructed B tree: \n\n";
    tree.printInorder();
    cout << "\n\n";

    do{ // Delete elements until user chooses to stop
        cout << "Enter key to delete: ";
        cin >> key;
        tree.remove(key);
        cout << "B-Tree after deleting " << key << "\n";
        tree.printInorder();
        cout << "\n\nMore (y/n)? ";
        cin >> choice;
        cout << "\n";
    }while(choice == 'y');
    return 0;
}
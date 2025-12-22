#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "node.h"

template <class T>
class Tree {
    private:
        // maintain the tree
        node<T>*  leftSingle(node<T>* root);
        node<T>*  leftDouble(node<T>* root);
        node<T>* rightSingle(node<T>* root);
        node<T>* rightDouble(node<T>* root);

    public:
        node<T>* head;

        Tree() {head = NULL;}
        Tree(T i) {head = new node<T>(i);}      // Create first node
        ~Tree() {delete head;}                  // Traverse tree and delete from bottom up

        // get info about the tree
        int height(node<T>* root);              // how far down a node is
        int weight(node<T>* root);              // measure the balance of a node's subtrees

        // main functions of the tree
        void insert(T data); // find the place for a new element
        node<T>* insert(node<T>* root, T data); // find the place for a new element
        void remove(node<T>* &root, T data);    // search for given piece of data and remove it's node 
        node<T>* balance(node<T>* root);        // balance tree

        // Displaying the tree
        void display(node<T>* temp, int level); 
        void printIn(node<T>* temp);            // Print tree in order
        void printPre(node<T>* temp);
        void printPost(node<T>* temp);
};

// Height of a tree or subtree
template <class T>
int Tree<T>::height(node<T>* root) {
    int h = 0;
    if (root != NULL) {
        int hLeft  = height(root->left);
        int hRight = height(root->right);
        h = ((hLeft > hRight) ? hLeft : hRight) + 1;
    }
    return h;
}

// Height difference
template <class T>
int Tree<T>::weight(node<T>* root) {
    int hLeft  = height(root->left);
    int hRight = height(root->right);
    return hLeft - hRight; // weight
}

/*
    a
     \
      b
       \
        c
    -->
      b
     / \
    a   c
*/
template <class T>
node<T>* Tree<T>::leftSingle(node<T>* a) { // ll
    // b becomes the new root
    node<T>* b = a->left;
    // a takes b's left child as its right child
    a->left = b->right;
    // b takes a as its left child
    b->right = a;
    return b;
}

/*
        c
       /
      a
       \
        b
    -->
        c
       /
      b
     /
    a
    -->
      b
     / \
    a   c
*/
template <class T>
node<T>* Tree<T>::leftDouble(node<T>* a) { // lr
    // right rotation on c (right)
    node<T>* b = a->left;
    a->left = rightSingle(b);
    // left rotation on a
    return leftSingle(a);
}

/*
        c
       /
      b
     /
    a
    -->
      b
     / \
    a   c
*/
template <class T>
node<T>* Tree<T>::rightSingle(node<T>* c) { // rr
    // b becomes the new root
    node<T>* b = c->right;
    // c takes b's right child as its left child
    c->right = b->left;
    // b takes c as its right child
    b->left = c;

    return b;
}

/*
    a
     \
      c
     /
    b
    -->
    a
     \
      b
       \
        c
    -->
      b
     / \
    a   c
*/
template <class T>
node<T>* Tree<T>::rightDouble(node<T>* a) { // rl
    // right rotation on c (right tree)
    node<T>* b = a->right;
    a->right = leftSingle(b);
    // left rotation on a
    return rightSingle(a);
}

template <class T>
void Tree<T>::insert(T data) {
    head = insert(head, data);
}
template <class T>
node<T>* Tree<T>::insert(node<T>* root, T data) {
    if (root == NULL) {
        root = new node<T>(data);
        return root;
    } else if (data < root->data) {
        root->left = insert(root->left, data);
        root = balance(root);
    } else if (data >= root->data) {
        root->right = insert(root->right, data);
        root = balance(root);
    }
    return root;
}

// took 3 tries ;-;
template <class T>
void Tree<T>::remove(node<T>* &root, T data) {
    // recur it
    // base condition
    if (root == NULL)
        return;

    // Go left for smaller data
    if (data < root->data) 
        remove(root->left, data);
    // Go right for bigger data
    else if (data > root->data)
        remove(root->right, data);
    // data node found
    else {
        // Case 1: no children
        if (root->left == NULL && root->right == NULL) {
            delete root;
            root = NULL;
        }
        // Case 2: 2 children
        else if (root->left && root->right) {
            // find next node less than root's data
            node<T>* nextLess = root->left;
            while (nextLess->right != NULL) {
                nextLess = nextLess->right;
            }

            // move lesser data to root
            root->data = nextLess->data;

            // delete lesser node
            remove(root->left, nextLess->data);
        }
        // Case 3: 1 child
        else {
            // remember current node
            node<T>* current = root;
            // move child up to this root node
            root = (root->left) ? root->left : root->right;
            // delete the now previous root node
            delete current;
        }
    }
}

template <class T>
node<T>* Tree<T>::balance(node<T>* root) {
    int w = weight(root);
    if (w > 1) { // right heavy
        if (weight(root->left) > 0) 
            root = leftSingle(root);
        else
            root = leftDouble(root);
    } else if (w < -1) {
        if (weight(root->right) > 0)
            root = rightDouble(root);
        else
            root = rightSingle(root);
    }
    return root;
}

template <class T>
void Tree<T>::display(node<T>* temp, int level){
    int i;
    if (temp!=NULL){
        display(temp->right, level + 1);
        cout<<endl;
        if (temp == head)
        cout<<"R -> ";
        for (i = 0; i < level && temp != head; i++)
            cout<<"     ";
        cout<<temp->data;
        display(temp->left, level + 1);
    }
}

template <class T>
void Tree<T>::printIn(node<T>* temp) {
    if (temp != NULL) {
        printIn(temp->left);
        cout << temp->data << ' ';
        printIn(temp->right);
    }
}

template <class T>
void Tree<T>::printPre(node<T>* temp) {
    if (temp != NULL) {
        cout << temp->data << ' ';
        printIn(temp->left);
        printIn(temp->right);
    }
}

template <class T>
void Tree<T>::printPost(node<T>* temp) {
    if (temp != NULL) {
        printIn(temp->left);
        printIn(temp->right);
        cout << temp->data << ' ';
    }
}


#endif /* TREE_H */
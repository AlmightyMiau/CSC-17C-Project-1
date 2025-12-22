#ifndef NODE_H
#define NODE_H

#include <cstdlib>
#include <iostream>
using namespace std;

template <class T>
struct node{
    T data;
    node *left;
    node *right;
    node(T d) {
        data = d;
        left  = NULL;
        right = NULL;
    }
    ~node() {
        if (left != NULL)
            delete left;
        if (right != NULL)
            delete right;
    }
};

#endif /* NODE_H */


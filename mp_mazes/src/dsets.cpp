/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    // Start with every element at -1
    for (int i = 0; i < num; i++) {
        sets_.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    int root = sets_[elem];

    // Check if reached the root of the subtree that contains elem, if so return elem
    if (root <= -1) {
        return elem;
    }

    // Find the root of the subtree that contains elem
    while (sets_[root] >= 0) {
        root = sets_[root];
    }

    // Compress path to root element
    sets_[elem] = root;
    return root;
}

void DisjointSets::setunion(int a, int b) {
    // Find the root of the subtree that contains a and contains b 
    int rootA = find(a);
    int rootB = find(b);

    // Check if the sets are already unionized, if so return
    if (rootA == rootB) {
        return;
    }

    // Find the (positive) sizes of the sets
    int sizeA = -1 * sets_[rootA];
    int sizeB = -1 * sets_[rootB];

    // Add sizes together
    int size = sizeA + sizeB;

    // Compare set sizes
    if (sizeA >= sizeB) {
        // If rootA's size is greater, a's parent element is the root
        sets_[rootA] = -1 * size;

        // b's parent element is the new root
        sets_[rootB] = rootA;
    } else {
        // If rootB's size is greater, b's parent element is the root
        sets_[rootA] = rootB;

        // a's parent element is the new root
        sets_[rootB] = -1 * size;
    }
}

int DisjointSets::size(int elem) {
    // Find the root of the subtree that contains elem and return its (postive) size
    int root = find(elem);
    return (-1 * sets_[root]);
}
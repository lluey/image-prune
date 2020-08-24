#include "qtvar.h"

bool qtvar::prunable(Node * root, int tol) {
    /* Your code here! */
    return root->var < tol;
}

int qtvar::pruneSize(int tol) {
    if(tol == 0) {
        return 0;
    }
    return quadtree::pruneSize(tol);
}

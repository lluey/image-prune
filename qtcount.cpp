#include "qtcount.h"
/* function prunable returns true if all of the leaf nodes in root's 
        * subtree have color within tol of root->avg. Distance is measured as 
        * square distance over the color channels: (r1-r2)^2 + (g1-g2)^2 + (b1-b2)^2 
        * and "within" is inclusive of tol.
        */
bool qtcount::prunable(Node * root, int tol) {
    /* Your code here */
    RGBAPixel rAvg = root->avg;
    return prunableNode(rAvg, root, tol);
    // return prunableNode(rAvg, root->NW, tol) && prunableNode(rAvg, root->NE, tol)
    //     && prunableNode(rAvg, root->SW, tol) && prunableNode(rAvg, root->SE, tol);
}
bool qtcount::prunableNode(RGBAPixel rAvg, Node * node, int tol) {
    if(node == NULL) {
        return true;
    }
    RGBAPixel nAvg = node->avg;
    int dist = pow((rAvg.r-nAvg.r), 2) + pow((rAvg.g-nAvg.g), 2) + pow((rAvg.b-nAvg.b), 2);
    bool prune = (dist <= tol);

    if(!prune) {
        return false;
    }
    if(node->NW == NULL) {
        return prune;
    }
    return prunableNode(rAvg, node->NW, tol) && prunableNode(rAvg, node->NE, tol)
        && prunableNode(rAvg, node->SW, tol) && prunableNode(rAvg, node->SE, tol);  

}
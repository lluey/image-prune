/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"

using namespace std;


// Node constructor, given.
quadtree::Node::Node(pair<int,int> ul, int d,RGBAPixel a, double v)
	:upLeft(ul),dim(d),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

// quadtree destructor, given.
quadtree::~quadtree() {
	clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree & other) {
	copy(other);
}
// quadtree assignment operator, given.
quadtree & quadtree::operator=(const quadtree & rhs) {
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}


quadtree::quadtree(PNG & imIn) {
	PNG im(imIn);
	int min = std::min(im.height(), im.width());
	int d = (int)log2(min);
	int size = pow(2, d);
	im.resize(size, size);  

	stats s(im);
	pair<int,int> c(0,0);
	root = buildTree(s, c, d);
	edge = size;
}


quadtree::Node * quadtree::buildTree(stats & s, pair<int,int> & ul, int dim) {
	RGBAPixel avg = s.getAvg(ul, dim);
	double var = s.getVar(ul, dim);
	Node* newRoot = new Node(ul, dim, avg, var);
	if (dim == 0) {
		return newRoot;
	}
	int newD = dim - 1;
	int length = pow(2, newD);
	pair<int,int> neul(ul.first + length, ul.second);
	pair<int,int> swul(ul.first, ul.second + length);
	pair<int,int> seul(ul.first + length, ul.second + length);
	
	newRoot->NW = buildTree(s, ul, newD);
	newRoot->SW = buildTree(s, swul, newD);
	newRoot->NE = buildTree(s, neul, newD);
	newRoot->SE = buildTree(s, seul, newD);
	
	return newRoot;
}

void quadtree::addPixels(quadtree::Node* parent, PNG & pixels) {
	if(parent == NULL) {
		return;
	}
	if(parent->NW == NULL) {
		int x = parent->upLeft.first;
		int y = parent->upLeft.second;
		int size = pow(2, parent->dim);
		for(int i = x; i < x+size; i++) {
			for(int j = y; j < y+size; j++) {
				RGBAPixel * pixel = pixels.getPixel(i,j);
				*pixel = parent->avg;
			}
		}
	}
	else {
		addPixels(parent->NW, pixels);
		addPixels(parent->NE, pixels);
		addPixels(parent->SW, pixels);
		addPixels(parent->SE, pixels);
	}	
}

PNG quadtree::render() { 
	PNG result = PNG(edge, edge);
	addPixels(root, result);
	return result;
}

int quadtree::idealPrune(int leaves){
	int newLeaves = leaves;
	if(newLeaves > 3) {
		while((newLeaves-1) % 3 != 0) {
			newLeaves--;
		}	
	}	
	else {
		newLeaves = 1;
	}
	return idealHelper(newLeaves, (int)root->var);
}

int quadtree::idealHelper(int leaves, int tol) {
	int lo = 0;
	int hi = tol;
	int min = 0;
	int mid = 0;
	while(lo <= hi) {
		mid = (lo + hi)/2;
		int size = pruneSize(mid);
		if(leaves == size) {
			min = mid;
		}
		if (leaves > size || leaves == size) {
			hi = mid - 1;
		}
		else {
			lo = mid + 1;
		}
	}	
	return min;
}

int quadtree::pruneSize(int tol){
	return sizeHelper(root, tol);
}

int quadtree::sizeHelper(Node * node, int tol) {
	if(node == NULL) {
		return 0;
	}
	if(node->NW == NULL||prunable(node, tol)) {
		return 1;
	}
	return sizeHelper(node->NW, tol) + sizeHelper(node->NE, tol) + sizeHelper(node->SW, tol) + sizeHelper(node->SE, tol);
}

void quadtree::prune(int tol){
	pruneHelper(root, tol);
}

void quadtree::pruneHelper(quadtree::Node* node, int tol) {
	if(node == NULL || node->NW == NULL) {
		return;
	}
	if(prunable(node, tol)) {
		clear(node->NW);
		clear(node->NE);
		clear(node->SW);
		clear(node->SE);
		node->NW = NULL;
		node->NE = NULL;
		node->SW = NULL;
		node->SE = NULL;
		return;
	}
	pruneHelper(node->NW,tol);
	pruneHelper(node->NE,tol);
	pruneHelper(node->SW,tol);
	pruneHelper(node->SE,tol);
}

void quadtree::clear() {
	clear(root);
}

void quadtree::clear(Node* & subNode) {
	if(subNode != NULL) {
		clear(subNode->NW);
		clear(subNode->NE);
		clear(subNode->SW);
		clear(subNode->SE);
		delete subNode;
		subNode = NULL;
	}
}

void quadtree::copy(const quadtree & orig){
	root = copyHelper(orig.root);
	edge = orig.edge;
}

quadtree::Node * ::quadtree::copyHelper(Node* subNode) {
	if(subNode == NULL) {
		return NULL;
	}
	Node* newNode = new Node(subNode->upLeft, subNode->dim, subNode->avg, subNode->var);
	newNode->NW = copyHelper(subNode->NW);
	newNode->NE = copyHelper(subNode->NE);
	newNode->SW = copyHelper(subNode->SW);
	newNode->SE = copyHelper(subNode->SE);
	return newNode;
}


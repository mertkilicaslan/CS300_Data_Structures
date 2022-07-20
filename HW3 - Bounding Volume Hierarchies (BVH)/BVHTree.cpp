// // Mert Kilicaslan - Bounding Volume Hierarchies (BVH) class implementation
#include "BVHTree.h"

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}

std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}

void destroyRecursive(BVHTreeNode* t) {
	// Internal method to make subtree empty
	if (t != NULL) {
		destroyRecursive(t->leftChild);
		destroyRecursive(t->rightChild);
		delete t;
		t = NULL;
	}
}

std::vector<std::string> getCollidingObjectsRec(BVHTreeNode* t, AABB object, std::vector<std::string>& vec) {
	// 1. If it’s a leaf, you will add it to the vector of names of collided members
	// 2. If it’s a branch, you will check its children
	if (t != NULL) {
		getCollidingObjectsRec(t->leftChild, object, vec);

		//At each node, if the node’s AABB overlaps with location, add to the vector
		if (t->aabb.collide(object) && t->isLeaf)
			vec.push_back(t->name);

		getCollidingObjectsRec(t->rightChild, object, vec);
	}
	return vec;
}

BVHTree::BVHTree() {
	root = NULL;
}

void BVHTree::addBVHMember(AABB objectArea, std::string name) {
	// The tree is empty
	if (root == NULL) {
		root = new BVHTreeNode(objectArea, name, true);
		map[name] = root;
	}

	// There is exactly one node in the tree
	else if (root != NULL && root->leftChild == NULL && root->rightChild == NULL) {
		BVHTreeNode* branch = new BVHTreeNode(root->aabb + objectArea, "branch", false);
		BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, true);
		map[name] = newNode;

		newNode->parent = branch;
		branch->leftChild = newNode;

		root->parent = branch;
		branch->rightChild = root;

		root = branch;
	}
	
	// There is more than one node in the tree
	else {
		int increaseInRightTreeSize, increaseInLeftTreeSize;
		BVHTreeNode* existingLeaf = root;
		BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, true);
		map[name] = newNode;
	
		// Attempt to place new nodes in locations that will minimize the increase in other nodes’ areas
		while (existingLeaf->leftChild != NULL) {
			increaseInRightTreeSize = AABB::unionArea(newNode->aabb, existingLeaf->rightChild->aabb) - existingLeaf->rightChild->aabb.getArea();
			increaseInLeftTreeSize = AABB::unionArea(newNode->aabb, existingLeaf->leftChild->aabb) - existingLeaf->leftChild->aabb.getArea();

			if (increaseInRightTreeSize < increaseInLeftTreeSize) 
				existingLeaf = existingLeaf->rightChild;
			else 
				existingLeaf = existingLeaf->leftChild;			
		}

		// Creating a branch node
		BVHTreeNode* branch = new BVHTreeNode(existingLeaf->aabb + objectArea, "branch", false);
		newNode->parent = branch;
		branch->leftChild = newNode;

		if(existingLeaf->parent->rightChild != existingLeaf)
			existingLeaf->parent->leftChild = branch;
		else
			existingLeaf->parent->rightChild = branch;

		branch->parent = existingLeaf->parent;
		branch->rightChild = existingLeaf;
		existingLeaf->parent = branch;


		// The parent node as well as all of the parent’s ancestors must adjust their area to fit the new child node
		BVHTreeNode* iter = branch;
		while (iter->parent != NULL) {
			if (iter->parent->rightChild == iter)
				iter->parent->aabb = iter->aabb + iter->parent->leftChild->aabb;
			else
				iter->parent->aabb = iter->aabb + iter->parent->rightChild->aabb;

			iter = iter->parent;
		}
	}
}

void BVHTree::removeBVHMember(std::string name) {
	BVHTreeNode* toRemovePtr = map[name];
	map.erase(name);

	// Depth: 0
	if (toRemovePtr->parent == NULL) {
		delete toRemovePtr;
		toRemovePtr = NULL;
		return;
	}

	// Find out where the sibling came from, i.e. whether the sibling is the right child or the left child of its parent (this is for depth 1 or more)
	BVHTreeNode* sibling;
	if (toRemovePtr->parent->rightChild == toRemovePtr)
		sibling = toRemovePtr->parent->leftChild;
	else
		sibling = toRemovePtr->parent->rightChild;

	// Depth: 1
	if (toRemovePtr->parent->parent == NULL) {
		delete toRemovePtr->parent;
		toRemovePtr->parent = NULL;

		delete toRemovePtr;
		toRemovePtr = NULL;

		root = sibling;
	}

	// Depth: 2 or more
	else {
		// The sibling's grandparent now becomes sibling's parent
		if (sibling->parent->parent->rightChild == sibling->parent)
			sibling->parent->parent->rightChild = sibling;
		else
			sibling->parent->parent->leftChild = sibling;

		sibling->parent = sibling->parent->parent;

		delete toRemovePtr->parent;
		toRemovePtr->parent = NULL;

		destroyRecursive(toRemovePtr);

		// The parent node as well as all of the parent’s ancestors must adjust their area to fit the new child node
		BVHTreeNode* iter = sibling;
		while (iter->parent != NULL) {
			if (iter->parent->rightChild == iter)
				iter->parent->aabb = iter->aabb + iter->parent->leftChild->aabb;
			else
				iter->parent->aabb = iter->aabb + iter->parent->rightChild->aabb;

			iter = iter->parent;
		}
	}
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation) {
	BVHTreeNode* toMovePtr = map[name];

	// Checking if the new location is inside the parent domain
	if (newLocation.minX < toMovePtr->parent->aabb.minX || newLocation.minY < toMovePtr->parent->aabb.minY ||
		newLocation.maxX > toMovePtr->parent->aabb.maxX || newLocation.maxY > toMovePtr->parent->aabb.maxY) {
		removeBVHMember(name);
		addBVHMember(newLocation, name);
	}
	// The new location is covered by the parent of the node
	else
		toMovePtr->aabb = newLocation;
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object) {
	std::vector<std::string> vec;

	// This function takes an AABB object and will store the names of all the members that collide with this object
	return getCollidingObjectsRec(root, object, vec);
}

BVHTree::~BVHTree(){
	destroyRecursive(root);
}

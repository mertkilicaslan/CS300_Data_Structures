#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include <iostream>
using namespace std;

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode {
    Comparable element;
    BinaryNode* left;
    BinaryNode* right;

    BinaryNode(const Comparable& theElement, BinaryNode* lt, BinaryNode* rt) :
            element(theElement),
            left(lt),
            right(rt) {}

    friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree {
public:
    explicit BinarySearchTree(const Comparable& notFound);
    BinarySearchTree(const BinarySearchTree& rhs);
    ~BinarySearchTree();
    BinarySearchTree();

    const Comparable& findMin() const;
    const Comparable& findMax() const;
    const Comparable& find(const Comparable& x) const;
    bool isEmpty() const;
    void printTree() const;

    void makeEmpty();
    void insert(const Comparable& x);
    void remove(const Comparable& x);
   
    Comparable* FindDetTable(Comparable& toBeFound);
    const BinarySearchTree& operator=(const BinarySearchTree& rhs);

private:
    BinaryNode<Comparable>* root;
    const Comparable ITEM_NOT_FOUND;

    const Comparable& elementAt(BinaryNode<Comparable>* t) const;

    void insert(const Comparable& x, BinaryNode<Comparable>*& t) const;
    void remove(const Comparable& x, BinaryNode<Comparable>*& t) const;
    BinaryNode<Comparable>* findMin(BinaryNode<Comparable>* t) const;
    BinaryNode<Comparable>* findMax(BinaryNode<Comparable>* t) const;
    BinaryNode<Comparable>* find(const Comparable& x, BinaryNode<Comparable>* t) const;
    void makeEmpty(BinaryNode<Comparable>*& t) const;
    void printTree(BinaryNode<Comparable>* t) const;
    BinaryNode<Comparable>* clone(BinaryNode<Comparable>* t) const;
};




// Construct the tree
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable& notFound) :
    ITEM_NOT_FOUND(notFound),
    root(nullptr){}


template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree() {
    root = NULL;
}

/*
   Internal method to get element field in node t.
   Return the element field or ITEM_NOT_FOUND if t is NULL.
 */
template <class Comparable>
const Comparable& BinarySearchTree<Comparable>::elementAt(BinaryNode<Comparable>* t) const {
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

/*
   Find item x in the tree.
   Return the matching item or ITEM_NOT_FOUND if not found.
 */
template <class Comparable>
const Comparable& BinarySearchTree<Comparable>::find(const Comparable& x) const {
    return elementAt(find(x, root));
}

/*
   Internal method to find an item in a subtree.
   x is item to search for.
   t is the node that roots the tree.
   Return node containing the matched item.
 */
template <class Comparable>
BinaryNode<Comparable>* BinarySearchTree<Comparable>::find(const Comparable& x, BinaryNode<Comparable>* t) const {
    if (t == NULL)
        return NULL;
    else if (x < t->element)
        return find(x, t->left);
    else if (t->element < x)
        return find(x, t->right);
    else
        return t;    // Match
}

/*
   Find the smallest item in the tree.
   Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Comparable>
const Comparable& BinarySearchTree<Comparable>::findMin() const {
    return elementAt(findMin(root));
}

/*
  Internal method to find the smallest item in a subtree t.
  Return node containing the smallest item.
*/
template <class Comparable>
BinaryNode<Comparable>* BinarySearchTree<Comparable>::findMin(BinaryNode<Comparable>* t) const {
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

/*
   Find the largest item in the tree.
   Return largest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable& BinarySearchTree<Comparable>::findMax() const {
    return elementAt(findMax(root));
}

/*
  Internal method to find the largest item in a subtree t.
  Return node containing the largest item.
*/
template <class Comparable>
BinaryNode<Comparable>* BinarySearchTree<Comparable>::findMax(BinaryNode<Comparable>* t) const {
    if (t != NULL)
        while (t->right != NULL)
            t = t->right;
    return t;
}

/*
   Insert x into the tree; duplicates are ignored.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable& x) {
    insert(x, root);
}

template <class Comparable>
Comparable* BinarySearchTree<Comparable>::FindDetTable(Comparable& tbf) {
    BinaryNode<Comparable>* found = find(tbf, root);
    return &(found->element);
}

/*
template <class HashedObj>
HashedObj* HashTable<HashedObj>::FindDetTable(HashedObj& element) {
    int i = findPos(element);
    return &(array[i].element);
}
*/


/*
   Internal method to insert into a subtree.
   x is the item to insert.
   t is the node that roots the tree.
   Set the new root.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable& x, BinaryNode<Comparable>*& t) const {
    if (t == NULL) //  create a new node at the right place
        t = new BinaryNode<Comparable>(x, NULL, NULL);
    else if (x < t->element)
        insert(x, t->left);  // insert at the left or 
    else if (t->element < x)
        insert(x, t->right);  // right subtree
    else
        ;  // Duplicate; do nothing
}

/*
   Remove x from the tree. Nothing is done if x is not found.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable& x) {
    remove(x, root);
}

/*
   Internal method to remove from a subtree.
   x is the item to remove.
   t is the node that roots the tree.
   Set the new root.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable& x, BinaryNode<Comparable>*& t) const {
    if (t == NULL)
        return;   // Item not found; do nothing
    if (x < t->element)
        remove(x, t->left);
    else if (t->element < x)
        remove(x, t->right);
    else if (t->left != NULL && t->right != NULL) { // Two children
        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    }
    else {// one or no children
        BinaryNode<Comparable>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }
}

/*
   Make the tree logically empty.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty() {
    makeEmpty(root);
}

/*
  Internal method to make subtree empty.
 */
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty(BinaryNode<Comparable>*& t) const {
    if (t != NULL) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

/*
   Print the tree contents in sorted order.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::printTree() const {
    if (isEmpty())
        cout << "Empty tree" << endl;
    else
        printTree(root);
}

/*
  Internal method to print a subtree rooted at t in sorted order.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::printTree(BinaryNode<Comparable>* t) const {
    if (t != NULL) {
        printTree(t->left);
        cout << t->element << endl;
        printTree(t->right);
    }
}

/*
   Destructor for the tree.
*/
template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree() {
    makeEmpty();
}

/*
   Copy constructor.
*/
template <class Comparable>
BinarySearchTree<Comparable>::
BinarySearchTree(const BinarySearchTree<Comparable>& rhs)
    : root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND) {
    *this = rhs;
}


/*
   Internal method to clone subtree.
*/
template <class Comparable>
BinaryNode<Comparable>* BinarySearchTree<Comparable>::clone(BinaryNode<Comparable>* t) const {
    if (t == NULL)
        return NULL;
    else
        return new BinaryNode<Comparable>(t->element, clone(t->left), clone(t->right));
}

template <class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const {
    return (root == NULL) ? true : false;
}

template <class Comparable>
const BinarySearchTree<Comparable>& BinarySearchTree<Comparable>::operator=(const BinarySearchTree& rhs) {
    if (this != &rhs) {
        makeEmpty();
        root = clone(rhs.root);
    }
    return *this;
}

#endif
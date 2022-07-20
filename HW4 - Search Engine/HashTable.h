#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <vector>
#include <iostream>

template <class HashedObj>
class HashTable{
public:
    explicit HashTable(const HashedObj& notFound, int size = 52);
    HashTable(const HashTable& rhs): 
        ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
        array(rhs.array), 
        currentSize(rhs.currentSize) {}

    const HashedObj& find(const HashedObj& x) const;
    void insert(const HashedObj& x);
    void remove(const HashedObj& x);

    HashedObj* FindDetTable(HashedObj& element);
    void AfterPreporcessing();

    enum EntryType {ACTIVE, EMPTY, DELETED};

private:
    struct HashEntry{
        HashedObj element;
        EntryType info;

        HashEntry(const HashedObj& e = HashedObj(), EntryType i = EMPTY): 
            element(e), info(i) {}
    };

    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;

    bool isActive(int currentPos) const;
    int findPos(const HashedObj& x) const;
    void rehash();
};

bool isPrime(unsigned int n){
    for (unsigned int i = 2; i < n; i++)
        if (n % i == 0)
            return false;
    return true;
}

int nextPrime(unsigned const int & n) {
    for (unsigned int i = n + 1; i < INT_MAX; i++)
        if (isPrime(i)) 
            return i; 
}

int myhash(const string& key, int tableSize){
    int hashVal = 0;
   
    for (int i = 0; i < key.length();i++)
    	hashVal = 37 * hashVal + key[ i ];
    
    hashVal = hashVal % tableSize;
    
    if (hashVal < 0)
    	hashVal = hashVal + tableSize;
    return hashVal;
   
}

/*
  Construct the hash table.
 */
template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj& notFound, int size):
    ITEM_NOT_FOUND(notFound),
    array(nextPrime(size)){
}

/**
 * Method that performs quadratic probing resolution.
 * Return the position where the search for x terminates.
 */
template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj& x) const{
    int collisionNum = 0;
    int currentPos = myhash(x.word, array.size());

    while (array[currentPos].info != EMPTY && array[currentPos].element != x){
        currentPos += pow(++collisionNum, 2);        //add the difference
        if (currentPos >= array.size())              // perform the mod
            currentPos -= array.size();             // if necessary
    }
    return currentPos;
}

/*
   Return true if currentPos exists and is active.
*/
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const{
    return array[currentPos].info == ACTIVE;
}

/*
  Remove item x from the hash table.
  x has to be in the table
 */
template <class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj& x){
    int currentPos = findPos(x);
    if (isActive(currentPos))
        array[currentPos].info = DELETED;
}

/*
  Find item x in the hash table.
  Return the matching item, or ITEM_NOT_FOUND, if not found.
 */
template <class HashedObj>
const HashedObj& HashTable<HashedObj>::find(const HashedObj& x)const {
    int currentPos = findPos(x);
    if (isActive(currentPos))
        return array[currentPos].element;
    return ITEM_NOT_FOUND;
}

/*
   Insert item x into the hash table. If the item is
   already present, then do nothing.
 */
template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj& x){
    // Insert x as active
    int currentPos = findPos(x);
    if (isActive(currentPos))
        return;
    array[currentPos] = HashEntry(x, ACTIVE);

    // enlarge the hash table if necessary 
    if (++currentSize >= int(array.size() * .5))
        rehash();
}

/*
  Expand the hash table.
 */
template <class HashedObj>
void HashTable<HashedObj>::rehash(){

    vector<HashEntry> oldArray = array;

    // Create new double-sized, empty table
    array.resize(nextPrime(2 * oldArray.size()));
    for (unsigned int j = 0; j < array.size(); j++)
        array[j].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for (unsigned int i = 0; i < oldArray.size(); i++)
        if (oldArray[i].info == ACTIVE)
            insert(oldArray[i].element);

    cout << "rehashed..." << endl;

    cout << "previous table size: " << oldArray.size()
        << ", new table size: " << array.size()
        << ", current unique word count " << currentSize
        << ", current load factor: " << double(currentSize) / oldArray.size() << endl;
}

template <class HashedObj>
HashedObj* HashTable<HashedObj>::FindDetTable(HashedObj& element){
    int i = findPos(element);
    return &(array[i].element);
}

template<class HashedObj>
void HashTable<HashedObj>::AfterPreporcessing(){
    currentSize--;
    cout << endl;
    cout << "After preprocessing, the unique word count is " << currentSize << ". Current load ratio is " << double(currentSize) / array.size() << endl;
}


#endif // ! HASHTABLE_H

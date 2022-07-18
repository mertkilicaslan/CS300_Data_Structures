// Mert Kilicaslan - Ziv-Lempel Algorithm with Hash Tables (hash table class)
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
using namespace std;

template <class HashedObj>
class HashTable {

public:

	HashTable<HashedObj>(const HashedObj& notFound, int size = 4096); // Constructor for hash table with size 4096

	void insert(const HashedObj& x, int& y); // If element doesn't present, insert the element x with key value y into the table 
	int find(const HashedObj& x);			 // Find item x in the hash table, return the matching item, if cannot find returns -1
	bool isEmpty();							 // Checks wheter the table has space to be mapped

private:

	int findPos(const HashedObj& x) const; // Return the position where the search for x terminates

	struct HashEntry {
		HashedObj element; // Generic template object
		int keyValue;	   // Key value for dictionary
	};

	int unsigned currentSize;		// Current hash table size
	const HashedObj ITEM_NOT_FOUND; // Default values in the hash table
	vector<HashEntry> hashArray;	// Hash Table

};


template<class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj& notFound, int size)
	:ITEM_NOT_FOUND(notFound), hashArray(size), currentSize(0) {

	// Filling the table with 'not found' objects
	for (unsigned int i = 0; i < hashArray.size(); i++)
		hashArray[i].element = ITEM_NOT_FOUND;
}

template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj& x, int& y) {
	int currentPos = findPos(x);

	// Element is not in the list, need for insertion with struct
	if (hashArray[currentPos].element != x) {
		hashArray[currentPos].element = x;
		hashArray[currentPos].keyValue = y;
		currentSize++;
	}
}

template<class HashedObj>
int HashTable<HashedObj>::find(const HashedObj& x) {
	int currentPos = findPos(x);
	// If object is found, return corresponding key value
	if (hashArray[currentPos].element == x) {
		return hashArray[currentPos].keyValue;
	}

	// Element does not found
	else {
		return -1;
	}
}

template<class HashedObj>
bool HashTable<HashedObj>::isEmpty() {
	return currentSize + 1 < hashArray.size(); // Checking wheter there is enough space for new mapping
}

template<class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj& x) const {
	int unsigned currentPos = hash<HashedObj>{}(x); // Template object
	currentPos %= hashArray.size();

	// Linear probing
	while (hashArray[currentPos].element != ITEM_NOT_FOUND && hashArray[currentPos].element != x) {
		currentPos++; // add the difference
		if (currentPos >= hashArray.size()) // apply mod if necessary
			currentPos = currentPos - hashArray.size();

	}
	return currentPos;
}

#endif
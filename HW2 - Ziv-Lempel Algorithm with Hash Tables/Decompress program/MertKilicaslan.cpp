// Mert Kilicaslan - Ziv-Lempel Algorithm with Hash Tables (decompression)
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

int main() {

	ifstream input("compout");	  // Input file for decompression
	ofstream output("decompout"); // Output file for compression

	const int size = 4096;	// Size for hash table
	string hashTable[size];	// Hash Table

	int key = 1;
	// Insertion of the ascii values
	for (key = 1; key < 256; key++) {
		string s;  // Char to string operations
		char c = char(key);
		s = c;
		hashTable[key] = s;
	}

	// Lempel Ziv Welch Algorithm
	int preNum, currentNum;
	string prev, current;

	// Getting first number to start the agorithm
	input >> currentNum;
	output << hashTable[currentNum];

	while (!input.eof()) {
		preNum = currentNum;
		input >> currentNum;

		// Entry already in the hash table
		if (!hashTable[currentNum].empty()) {
			output << hashTable[currentNum]; // Writing out the element

			prev = hashTable[preNum];
			current = hashTable[currentNum][0];

			hashTable[key] = prev + current;
			key++; // Add the difference
		}

		// Entry doesn't present in the hash table
		else {
			prev = hashTable[preNum];
			current = hashTable[preNum][0];

			output << prev + current; // Writing out the element

			hashTable[key] = prev + current;
			key++; // Add the difference
		}
	}

	output.close();
	input.close();

	return 0;
}

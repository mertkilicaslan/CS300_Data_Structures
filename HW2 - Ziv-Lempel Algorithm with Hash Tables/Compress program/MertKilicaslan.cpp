// Mert Kilicaslan - Ziv-Lempel Algorithm with Hash Tables (compression)
#include "HashTable.h"
#include <string>
#include <fstream>
#include <iostream>

int main() {

	HashTable<string> table("ITEM_NOT_FOUND"); // Hash Table
	ifstream input("compin");	// Input file for compression
	ofstream output("compout"); // Output file for decompression

	int key = 1;
	// Insertion of the ascii values
	for (key = 1; key < 256; key++) {
		string s; // Char to string operations
		char c = char(key);
		s = c;
		table.insert(s, key);
	}

	// Lempel Ziv Welch Algorithm
	char inputChar;
	string prevCurrent, prev, current;

	while (input.get(inputChar)) {
		current = inputChar;
		prevCurrent = prev + current;

		// Find item x in the hash table, return the matching item, if cannot find returns -1
		if (table.find(prevCurrent) != -1)
			prev = prevCurrent;

		else {
			// Insertion only happens there is enough space in the hash table
			if (table.isEmpty()) {
				table.insert(prevCurrent, key);
				key++;	// Add the difference
			}

			output << table.find(prev) << " "; // Writing out the key value
			prev = current;
		}
	}
	output << table.find(prev);
	output.close();
	input.close();

	return 0;
}
// Mert Kilicaslan - Search Engine
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

#include "BSTTree.h"
#include "HashTable.h"
using namespace std;


// Lowers the letters of the given string parameter (unique words need to be case insensitive)
void lowerTheWord(string & theWord) {
	for (unsigned int i = 0; i < theWord.length(); i++) 
		if (theWord[i] >= 'A' && theWord[i] <= 'Z')
			theWord[i] += ('a' - 'A');
}

// Numbers and punctuation must be treated as separators for words.
void seperateTheWord(string & theWord) {
	for (unsigned int i = 0; i < theWord.length(); i++)
		if (!(theWord[i] >= 'a' && theWord[i] <= 'z')) 
			theWord[i] = ' ';
}



struct DocumentItem {
	string documentName;
	int count;
};

struct WordItem {
	string word;
	vector<DocumentItem> documentAndOccurences;

	bool operator < (const WordItem& rhs) const {
		return this->word < rhs.word;
	}

	bool operator > (const WordItem& rhs) const {
		return this->word < rhs.word;
	}

	bool operator != (const WordItem& rhs) const {
		return this->word != rhs.word;
	}

	bool operator == (const WordItem& rhs) const {
		return this->word == rhs.word;
	}
};

// Checks if the currentFileName is in the documentAndOccurences vector of the WordItem obbject
bool doesFileNameExistInDocumentItem(const vector<DocumentItem> & documentAndOccurences, const string & currentFileName) {
	for (unsigned int i = 0; i < documentAndOccurences.size(); i++)
		if (currentFileName == documentAndOccurences[i].documentName)
			return true;
	return false;
}

// Returns the index of existed currentFileName which is in the documentAndOccurences vector of the WordItem obbject
int indexOfDocItem(const vector<DocumentItem> & documentAndOccurences, const string & currentFileName) {
	for (unsigned int i = 0; i < documentAndOccurences.size(); i++)
		if (currentFileName == documentAndOccurences[i].documentName)
			return i;
	return -1;
}

// Returns the vector of txt files containing the query
vector<string> queryExistInFile(const vector<WordItem> & wordItemQuery, const vector<string> & fileVector) {
	vector<string> docsContainAllQuery;
	for (unsigned int i = 0; i < fileVector.size(); i++) {
		int counter = 0;

		for (unsigned int j = 0; j < wordItemQuery.size(); j++) 
			for (unsigned int k = 0; k < wordItemQuery[j].documentAndOccurences.size(); k++)
				if (fileVector[i] == wordItemQuery[j].documentAndOccurences[k].documentName)
					counter++;

		if (counter == wordItemQuery.size())
			docsContainAllQuery.push_back(fileVector[i]);
	}
	return docsContainAllQuery;
}

// Prints the document names and the number of times each queried word appears in that document
void printDocsAndQuery(const vector<WordItem>& wordItemQuery, const vector<string>& docsContainAllQuery) {
	if (docsContainAllQuery.size() != 0) {
		for (unsigned int i = 0; i < docsContainAllQuery.size(); i++) {
			cout << "in Document " << docsContainAllQuery[i] << ", ";

			for (unsigned int j = 0; j < wordItemQuery.size(); j++) {
				if (wordItemQuery[j].word != "") {
					cout << wordItemQuery[j].word << " found ";

					for (unsigned int k = 0; k < wordItemQuery[j].documentAndOccurences.size(); k++) {
						if (docsContainAllQuery[i] == wordItemQuery[j].documentAndOccurences[k].documentName) {
							cout << wordItemQuery[j].documentAndOccurences[k].count;

							if (j != wordItemQuery.size() - 1)
								cout << " times, ";
							else
								cout << " times.";
						}
					}
				}
			}
			cout << endl;
		}
	}
	else
		cout << "No document contains the given query" << endl;
}

int main() {

	unsigned int numOfDocuments;
	cout << "Enter the number of input files: ";
	cin >> numOfDocuments;

	vector<string> fileVector;

	for (unsigned int i = 1; i <= numOfDocuments; i++) {
		string document;
		cout << "Enter " << i << ". file name: ";
		cin >> document;
		fileVector.push_back(document);
	}

	WordItem ITEM_NOT_FOUND;
	ITEM_NOT_FOUND.word = "ITEM_NOT_FOUND";

	// Creating data structures
	BinarySearchTree<WordItem> BST(ITEM_NOT_FOUND);
	HashTable<WordItem> HT(ITEM_NOT_FOUND);


	for (unsigned int i = 0; i < fileVector.size(); i++) {
		string currentFileName = fileVector[i];
		ifstream input;

		input.open(currentFileName.c_str());

		string word;
		while (input >> word) {
			lowerTheWord(word); // Lowers the letters of the given string parameter (unique words need to be case insensitive)
			seperateTheWord(word); //  Numbers and punctuation must be treated as separators for words.

			string splittedWord;
			stringstream ss(word);
			while (getline(ss, splittedWord, ' ')) {
				WordItem theWordItem;
				theWordItem.word = splittedWord;

				DocumentItem theDocumentItem;
				theDocumentItem.documentName = currentFileName;
			
				WordItem foundItemHT = HT.find(theWordItem);

				// Word item does not exist in data structures
				if (foundItemHT == ITEM_NOT_FOUND) {
					theDocumentItem.count = 1;

					theWordItem.documentAndOccurences.push_back(theDocumentItem);
					BST.insert(theWordItem);
					HT.insert(theWordItem);
				}

				// Word item already exists in data structures
				else {
					// Objects real location in data structures
					WordItem* realFoundItemBST = BST.FindDetTable(theWordItem);
					WordItem* realFoundItemHT = HT.FindDetTable(theWordItem);

					// Current file already exists in the WordItem's documentAndOccurences vector
					if (doesFileNameExistInDocumentItem(realFoundItemHT->documentAndOccurences, currentFileName)) {
						int theIndex = indexOfDocItem(realFoundItemHT->documentAndOccurences, currentFileName);

						realFoundItemBST->documentAndOccurences[theIndex].count++;
						realFoundItemHT->documentAndOccurences[theIndex].count++;
					}
					
					// Current file is new and does not exist in the WordItem's documentAndOccurences vector
					else {
						theDocumentItem.count = 1;

						realFoundItemBST->documentAndOccurences.push_back(theDocumentItem);
						realFoundItemHT->documentAndOccurences.push_back(theDocumentItem);
					}			
				}
			}
		}
		input.close();
	}

	cout << endl;
	HT.AfterPreporcessing();

	string line;

	cout << "Enter queried words in one line: ";
	cin.clear();
	cin.ignore();
	getline(cin, line);

	lowerTheWord(line); // Lowers the letters of the given string parameter (unique words need to be case insensitive)
	seperateTheWord(line); // Numbers and punctuation must be treated as separators for words.

	vector<WordItem> wordItemQueryBST;
	vector<WordItem> wordItemQueryHT;

	string splittedWord;
	stringstream ss(line);

	long long timeBST = 0;
	long long timeHT = 0;

	while (getline(ss, splittedWord, ' ')) {
		WordItem theWordItem;
		theWordItem.word = splittedWord;

		WordItem foundItemBST;
		WordItem foundItemHT;

		// Compute the time it takes to do the query with each data structure
		int k = 100;
		// QueryDocuments (with BST)
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < k; i++)
		{
			foundItemBST = BST.find(theWordItem);
		}
		auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
			(std::chrono::high_resolution_clock::now() - start);
		timeBST += (BSTTime.count() / k);

		// QueryDocuments (with hashtable)
		start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < k; i++)
		{
			foundItemHT = HT.find(theWordItem);
		}
		auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
			(std::chrono::high_resolution_clock::now() - start);
		timeHT += (HTTime.count() / k);

		// Add the found element to the vector to display it in the format
		wordItemQueryBST.push_back(foundItemBST);
		wordItemQueryHT.push_back(foundItemHT);
	}

	// Prints the document names and the number of times each queried word appears in that document
	vector<string> docsContainAllQueryBST = queryExistInFile(wordItemQueryBST, fileVector);
	printDocsAndQuery(wordItemQueryBST, docsContainAllQueryBST);

	vector<string> docsContainAllQueryHT = queryExistInFile(wordItemQueryHT, fileVector);
	printDocsAndQuery(wordItemQueryHT, docsContainAllQueryHT);


	// Display data structures timing with the speedup ratio of these timing values compared to each other
	cout << endl;
	cout << "Time: " << timeBST << endl << endl;
	cout << "Time: " << timeHT << endl;
	cout << "Speed up: " << double(timeBST) / timeHT << endl;

	return 0;
}

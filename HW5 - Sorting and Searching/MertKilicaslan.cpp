// Mert Kilicaslan - Sorting and Searching
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;


// Insertion Sort
template <class Comparable>
void insertionSort(vector <Comparable>& a) {
	int j;

	for (int p = 1; p < a.size(); p++) {
		Comparable tmp = a[p];

		for (j = p; j > 0 && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}

// Quick Sort
template <class Comparable>
void insertionSort(vector<Comparable>& a, int left, int right) {
	int j;
	for (int p = left + 1; p <= right; p++) {
		Comparable tmp = a[p];

		for (j = p; j > left && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}

// Quick Sort 2
template <class Comparable>
const Comparable& median3(vector<Comparable>& a, int left, int right) {
	int center = (left + right) / 2;
	if (a[center] < a[left])
		swap(a[left], a[center]);
	if (a[right] < a[left])
		swap(a[left], a[right]);
	if (a[right] < a[center])
		swap(a[center], a[right]);

	// Place pivot at position right - 1
	swap(a[center], a[right - 1]);
	return a[right - 1];
}

// Quick Sort 3
template <class Comparable>
void quicksort(vector<Comparable>& a, int left, int right) {
	if (left + 10 <= right) {
		Comparable pivot = median3(a, left, right);
		// Begin partitioning
		int i = left, j = right - 1;
		for (;;)
		{
			while (a[++i] < pivot) {}

			while (pivot < a[--j]) {}

			if (i < j)
				swap(a[i], a[j]);
			else
				break;
		}

		swap(a[i], a[right - 1]);   // Restore pivot
		quicksort(a, left, i - 1);       // Sort small elements
		quicksort(a, i + 1, right);    // Sort large elements
	}

	else  // Do an insertion sort on the subarray
		insertionSort(a, left, right);
}

// Quick Sort 4
template <class Comparable>
void quicksort(vector<Comparable>& a) {
	quicksort(a, 0, a.size() - 1);
}


// Merge Sort (in-place)
template <class Comparable>
void merge(vector<Comparable>& a,
	int leftPos, int rightPos, int rightEnd) {
	int leftEnd = rightPos - 1;

	// Main loop
	Comparable temp;
	while (leftPos <= leftEnd && rightPos <= rightEnd) {
		if (a[leftPos] > a[rightPos]) {

			temp = a[leftPos];
			a[leftPos] = a[rightPos];

			for (int i = rightPos; i > leftPos; i--)
				a[i] = a[i - 1];

			leftPos++;
			a[leftPos] = temp;
			leftEnd++;
			rightPos++;
		}

		else
			leftPos++;
	}
}

// Merge Sort 2
template <class Comparable>
void mergeSort(vector<Comparable>& a, int left, int right) {
	if (left < right) {
		int center = (left + right) / 2;
		mergeSort(a, left, center);
		mergeSort(a, center + 1, right);
		merge(a, left, center + 1, right);
	}
}

// Merge Sort 3
template <class Comparable>
void mergeSort(vector<Comparable>& a) {
	mergeSort(a, 0, a.size() - 1);
}

// Heap Sort
inline int leftChild(int i){
	return 2 * i + 1;
}

// Heap Sort 2 
template <class Comparable>  // for deleteMax
void percDown(vector<Comparable>& a, int i, int n){
	int child;			// a is the array, i is the position to percolate down from
	Comparable tmp;		// n is the logical size of the array

	for (tmp = a[i]; leftChild(i) < n; i = child){
		child = leftChild(i);
		if (child != n - 1 && a[child] < a[child + 1])
			child++;
		if (a[child] > tmp)
			a[i] = a[child];
		else
			break;
	}
	a[i] = tmp;
}

// Heap Sort 3
template <class Comparable>
void heapSort(vector<Comparable>& a){
	// buildHeap
	for (int i = a.size() / 2; i >= 0; i--)
		percDown(a, i, a.size());

	// sort
	for (int j = a.size() -1; j > 0; j--){
		swap(a[0], a[j]);    // swap max to the last pos.
		percDown(a, 0, j);		// re-form the heap
	}
}


// Prints the vector
template <class Comparable>
void printVector(const vector <Comparable>& vec) {
	for (unsigned int i = 0; i < vec.size(); i++)
		cout << vec[i] << endl;
}


// Uppers the letters of the given string parameter
void upperTheWord(string& theWord) {
	for (unsigned int i = 0; i < theWord.length(); i++)
		if (theWord[i] >= 'a' && theWord[i] <= 'z')
			theWord[i] -= ('a' - 'A');
}


int main() {

	string fileName;
	cout << "Please enter the contact file name:" << endl;
	cin >> fileName;

	cin.clear();
	cin.ignore();

	string nameToBeSearched;
	cout << "Please enter the word to be queried:" << endl;
	getline(cin, nameToBeSearched);

	ifstream input;
	input.open(fileName.c_str());

	vector<string> phoneBookVecQuickSort, phoneBookVecInsertionSort, phoneBookVecMergeSort, phoneBookVecHeapSort;
	string firstName, surname, phoneNum, city, infoLine;

	while (input >> firstName >> surname >> phoneNum >> city) {
		upperTheWord(firstName);
		upperTheWord(surname);

		infoLine = firstName + " " + surname + " " + phoneNum + " " + city;

		phoneBookVecQuickSort.push_back(infoLine);
		phoneBookVecInsertionSort.push_back(infoLine);
		phoneBookVecMergeSort.push_back(infoLine);
		phoneBookVecHeapSort.push_back(infoLine);	
	}

	cout << endl;
	cout << "Sorting the vector copies" << endl;
	cout << "======================================" << endl;

	int N = 1;

	// Quick Sort
	auto startQS = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++)
		quicksort(phoneBookVecQuickSort);

	auto executionTimeQS = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - startQS).count();
	cout << "Quick Sort Time: " << executionTimeQS / N << " Nanoseconds" << endl;


	// Insertion Sort
	auto startIS = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++)
		insertionSort(phoneBookVecInsertionSort);

	auto executionTimeIS = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - startIS).count();
	cout << "Insertion Sort Time: " << executionTimeIS / N << " Nanoseconds" << endl;


	// Merge Sort
	auto startMS = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++)
		mergeSort(phoneBookVecMergeSort);

	auto executionTimeMS = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - startMS).count();
	cout << "Merge Sort Time: " << executionTimeMS / N << " Nanoseconds" << endl;


	// Heap Sort
	auto startHS = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < N; i++)
		heapSort(phoneBookVecHeapSort);

	auto executionTimeHS = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - startHS).count();
	cout << "Heap Sort Time: " << executionTimeHS / N << " Nanoseconds" << endl;



	cout << endl;
	cout << "Searching for " << nameToBeSearched << endl;
	cout << "======================================" << endl;
	upperTheWord(nameToBeSearched);

	N = 100;
	
	// Sequential Search
	bool isFull = false;
	vector<string> resultsSequentialSearch;

	auto startSS = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < N; j++) {

		for (unsigned int i = 0; i < phoneBookVecInsertionSort.size(); i++) {
			string fullName = phoneBookVecInsertionSort[i].substr(0, phoneBookVecInsertionSort[i].find(" +"));

			if (nameToBeSearched == fullName) {
				resultsSequentialSearch.push_back(phoneBookVecInsertionSort[i]);
				isFull = true;
				break;
			}

			else if (fullName.substr(0, nameToBeSearched.length()) == nameToBeSearched)
				resultsSequentialSearch.push_back(phoneBookVecInsertionSort[i]);
		}

		if (j == N - 2)
			resultsSequentialSearch.clear();
	}
	auto executionTimeSS = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - startSS);



	// Binary Search
	vector<string> resultsBinarySearch;

	auto startBS = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < N; j++) {

		int startIndex = 0, midIndex = 0;
		int endIndex = phoneBookVecInsertionSort.size() - 1;
		int lastIndex = endIndex;

		// Full name search
		if (isFull) {
			while (startIndex <= endIndex) {
				midIndex = (startIndex + endIndex) / 2;
				string fullName = phoneBookVecInsertionSort[midIndex].substr(0, phoneBookVecInsertionSort[midIndex].find(" +"));

				if (fullName < nameToBeSearched)
					startIndex = midIndex + 1;

				else if (fullName > nameToBeSearched)
					endIndex = midIndex - 1;

				else if (fullName == nameToBeSearched){
					resultsBinarySearch.push_back(phoneBookVecInsertionSort[midIndex]);
					break;
				}
			}
		}

		// Partial name search
		else {
			while (startIndex <= endIndex) {
				midIndex = (startIndex + endIndex) / 2;
				string partialName = phoneBookVecInsertionSort[midIndex].substr(0, nameToBeSearched.length());

				if (partialName == nameToBeSearched) {
					while (midIndex >= 0 && phoneBookVecInsertionSort[midIndex].substr(0, nameToBeSearched.length()) == nameToBeSearched)
							midIndex = midIndex - 1;

					midIndex = midIndex + 1;

					while (midIndex <= lastIndex && phoneBookVecInsertionSort[midIndex].substr(0, nameToBeSearched.length()) == nameToBeSearched) {
						resultsBinarySearch.push_back(phoneBookVecInsertionSort[midIndex]);
						midIndex = midIndex + 1;
					}
					break;
				}

				else if (partialName < nameToBeSearched)
					startIndex = midIndex + 1;

				else
					endIndex = midIndex - 1;
			}
		}

		if (j == N - 2)
			resultsBinarySearch.clear();
	}
	
	auto executionTimeBS = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - startBS);



	// Print Binary Search
	cout << "Search results for Binary Search:" << endl;
	if (resultsBinarySearch.size() == 0)
		cout << nameToBeSearched << " does NOT exist in the dataset" << endl;
	else
		printVector(resultsBinarySearch);
	cout << endl;
	cout << "Binary Search Time: " << (1.0 * executionTimeBS.count()) / N << " Nanoseconds" << endl;

	cout << endl;

	// Print Sequential Search
	cout << "Search results for Sequential Search:" << endl;
	if (resultsSequentialSearch.size() == 0)
		cout << nameToBeSearched << " does NOT exist in the dataset" << endl;
	else
		printVector(resultsSequentialSearch);
	cout << endl;
	cout << "Sequential Search Time: " << (1.0 * executionTimeSS.count()) / N << " Nanoseconds" << endl << endl;

	// Speed-Up Search Alogrithms
	cout << "SpeedUp between Search Algorithms" << endl;
	cout << "======================================" << endl;
	cout << "(Sequential Search / Binary Search) SpeedUp = " << (1.0 * executionTimeSS) / executionTimeBS << endl;

	cout << endl;

	// Speed-Up Sorting Alogrithms
	cout << "SpeedUps between Sorting Algorithms" << endl;
	cout << "======================================" << endl;
	cout << "(Insertion Sort / Quick Sort) SpeedUp = " << (1.0 * executionTimeIS) / executionTimeQS << endl;
	cout << "(Merge Sort / Quick Sort) SpeedUp = " << (1.0 * executionTimeMS) / executionTimeQS << endl;
	cout << "(Heap Sort / Quick Sort) SpeedUp = " << (1.0 * executionTimeHS) / executionTimeQS << endl;

	return 0;
}
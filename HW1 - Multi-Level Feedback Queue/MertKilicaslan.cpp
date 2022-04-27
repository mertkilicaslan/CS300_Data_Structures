// Mert Kilicaslan - Multi-Level Feedback Queue (MLFQ)
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

struct process {
	int id;
	queue<char> status;
};

int main() {

	string folderName;
	cout << "Please enter the process folder name: ";
	cin >> folderName;
	cout << "When all processes are completed, you can find execution sequence in \"" + folderName + "/output.txt\"." << endl;

	// Getting input for configuration file that contains number of queues, processes and time period
	ifstream configInput;
	configInput.open(folderName + "/configuration.txt");

	int queueNum, processNum, cycle;
	configInput >> queueNum >> processNum >> cycle;
	configInput.close();

	// MLFQ Vector works as a container for process structs of queues 
	vector<queue<process>> MLFQ(queueNum);

	// Getting process files that contains the status which specifies what the process does in each time slice
	// When a process enters the system, it is placed at the highest priority (the topmost queue).
	ifstream processInput;
	for (int i = 1; i <= processNum; i++) {
		process tempProcess;
		tempProcess.id = i;

		char tempChar = ' ';
		processInput.open(folderName + "/p" + to_string(i) + ".txt");

		while (tempChar != '-') {
			processInput >> tempChar;
			tempProcess.status.push(tempChar);
		}

		MLFQ[0].push(tempProcess);
		processInput.close();
	}


	// MLFQ Algorithm Implementation
	ofstream outputFile;
	outputFile.open(folderName + "/output.txt");

	int cycleCounter = 0;
	for (unsigned int i = 0; i < MLFQ.size(); i++) {
		while (MLFQ[i].size() != 0) {

			// Process terminates it should be removed from the MLFQ structure
			if (MLFQ[i].front().status.size() == 2) {
				outputFile << "E, PC" << MLFQ[i].front().id << ", QX" << endl;

				MLFQ[i].pop();
			}

			// If a process gives up the CPU before its time slice is completed, it stays at the same priority level
			else if (MLFQ[i].front().status.front() == '0') {
				outputFile << MLFQ[i].front().status.front() << ", PC" << MLFQ[i].front().id << ", Q" << MLFQ.size() - i << endl;

				MLFQ[i].front().status.pop();
				MLFQ[i].push(MLFQ[i].front());
				MLFQ[i].pop();
			}

			// If a process uses the entire time slice while running
			else if (MLFQ[i].front().status.front() == '1') {

				// If process is at the lowest priority queue: It stays at the same priority level
				if (i + 1 == MLFQ.size()) {
					outputFile << MLFQ[i].front().status.front() << ", PC" << MLFQ[i].front().id << ", Q" << MLFQ.size() - i << endl;

					MLFQ[i].front().status.pop();
					MLFQ[i].push(MLFQ[i].front());
					MLFQ[i].pop();
				}

				// If process is not at the lowest priority queue: Its priority is reduced by one
				else {
					outputFile << MLFQ[i].front().status.front() << ", PC" << MLFQ[i].front().id << ", Q" << MLFQ.size() - i - 1 << endl;

					MLFQ[i].front().status.pop();
					MLFQ[i + 1].push(MLFQ[i].front());
					MLFQ[i].pop();
				}
			}

			cycleCounter++;

			// Cycle is reached by cycle counter: After some time period S, all the processes are moved to the topmost queue
			if (cycleCounter == cycle) {
				for (unsigned int j = 1; j < MLFQ.size(); j++) {
					while (MLFQ[j].size() != 0) {
						outputFile << "B, PC" << MLFQ[j].front().id << ", Q" << MLFQ.size() << endl;

						MLFQ[0].push(MLFQ[j].front());
						MLFQ[j].pop();
					}
				}

				cycleCounter = 0;
				i = 0;
			}
		}
	}

	return 0;
}
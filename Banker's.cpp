#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> calcNeed(vector<vector<int>> allocated, vector<vector<int>> maximum)
{
	// Calculation of needed matrix
	vector<vector<int>> need(allocated.size());
	for (int i = 0; i < need.size(); i++) {
		need[i].push_back(i);
		for (int j = 1; j < maximum[i].size(); j++) {
			int x = maximum[i][j];
			int y = allocated[i][j];
			need[i].push_back(x - y);
		}
	}
	return need;
}

void printArrs(vector<vector<int>>allocs, vector<vector<int>>maxRes, vector<vector<int>>needed) {
	cout << endl << "// Allocated || Max || Needed //" << endl;
	for (int i = 0; i < allocs.size(); i++) {
		cout << maxRes[i][0] << " | ";
		for (int j = 1; j < allocs[i].size(); j++) {
			cout << allocs[i][j] << " ";
		}
		cout << "|| ";
		for (int j = 1; j < allocs[i].size(); j++) {

			cout << maxRes[i][j] << " ";
		}
		cout << "|| ";
		for (int j = 1; j < allocs[i].size(); j++) {

			cout << needed[i][j] << " ";
		}
		cout << endl;

	}
}

bool isSafe(vector<vector<int>> allocated, vector<vector<int>> maximum, vector<int> available)
{
	// Calculation of needed matrix
	vector<vector<int>> need = calcNeed(allocated, maximum);
	//vector<vector<int>> temp = need;
	cout << endl << "// needed || available before || allocated || available after //" << endl;
	// search for processes that can be executed
	for (int i = 0; i < allocated.size(); i++) {
		// Feasability Check
		bool canExec = true;
		for (int j = 1; j < need[i].size(); j++) {
			canExec = canExec && (need[i][j] <= available[j]);
		}
		// If Feasable, execute
		if (canExec) {
			cout << endl << "Executed P#" << maximum[i][0] << endl;
			for (int j = 1; j < need[i].size(); j++) {

				cout << need[i][j] << " ";
			}
			cout << "|| ";
			for (int j = 1; j < need[i].size(); j++) {
				cout << available[j] << " ";
			}
			cout << "|| ";
			// Adding the Allocated resources To Available resources.
			for (int j = 1; j < available.size(); j++) {
				available[j] += allocated[i][j];
			}
			for (int j = 1; j < allocated[i].size(); j++) {
				cout << allocated[i][j] << " ";
			}
			cout << "|| ";
			for (int j = 1; j < need[i].size(); j++) {
				cout << available[j] << " ";
			}
			cout << endl;
			// Removing the executed process from the queue/list
			need.erase(need.begin() + i);
			allocated.erase(allocated.begin() + i);
			maximum.erase(maximum.begin() + i);
			// Resetting the counter to review the whole queue/list
			i = -1;
		}
	}
	// If all processes are executed, then the last process was safe
	// If one or more process(es) are left, then the last process was unsafe
	bool safe = allocated.size() == 0;
	if (safe) {
		cout << endl << "Safe" << endl;
	}
	else {
		cout << endl << "Unsafe" << endl;
	}
	return safe;
}

void randomDisAlloc(vector<vector<int>> & allocated, vector<int> & available) {
	int pNum = rand() % allocated.size();
	vector<int> disallocs(available.size(), 0);
	cout << endl << "disallocating P#" << allocated[pNum][0] << endl;
	for (int i = 1; i < disallocs.size(); i++) {
		int r = rand() % (allocated[pNum][i] + 1);
		cout << r << " ";
	}
	cout << endl;
	cout << endl << "randomly disallocated P#" << pNum << endl;
	for (int i = 1; i < available.size(); i++) {
		available[i] += disallocs[i];
		allocated[pNum][i] -= disallocs[i];
	}
}

void Banker() {
	int numP, numR, userMode, nIterations;
	cout << "Enter 0 for random numbers and 1 for User Input" << endl;
	cin >> userMode;
	cout << "Enter number of processes" << endl;
	cin >> numP;
	cout << "Enter number of resources" << endl;
	cin >> numR;
	cout << "Enter number of Iterations" << endl;
	cin >> nIterations;
	numR++;
	vector<int> avail(numR, 0);
	vector<vector<int>> maxRes(numP, vector<int>(numR, 0));
	vector<vector<int>> allocs(numP, vector<int>(numR, 0));
	cout << "Enter number of available of resources" << endl;
	for (int i = 1; i < numR; i++) {
		cin >> avail[i];
	}
	//For manual input
	if (userMode) {
		for (int i = 0; i < numP; i++) {
			cout << "Enter maximum number of resources needed for process#" << i << endl;
			maxRes[i][0] = i;
			for (int j = 1; j < numR; j++) {
				cin >> maxRes[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < numP; i++) {
			maxRes[i][0] = i;
			for (int j = 1; j < numR; j++) {
				int x = rand() % (avail[j] + 1);
				maxRes[i][j] = x;
			}
		}
	}

	cout << "\n////////////////////// - Generated/Input Processes - //////////////////////" << endl;
	cout << endl << "available" << endl;
	for (int j = 1; j < numR; j++) {
		cout << avail[j] << " ";
	}
	cout << endl;
	printArrs(allocs, maxRes, calcNeed(allocs, maxRes));
	cout << "\n////////////////////// -           End          - //////////////////////" << endl;


	for (int it = 0; it < nIterations; it++) {
		for (int i = 0; i < numP; i++) {
			cout << "\n////////////////////// - Before Process testing - //////////////////////" << endl;
			cout << endl << "available" << endl;
			for (int j = 1; j < numR; j++) {
				cout << avail[j] << " ";
			}
			cout << endl;
			printArrs(allocs, maxRes, calcNeed(allocs, maxRes));
			cout << "\n////////////////////// -           End          - //////////////////////" << endl;
			if (rand() % 2) {
				randomDisAlloc(allocs, avail);
				cout << "\n////////////////////// - System After Process Deallocation - //////////////////////" << endl;
				cout << endl << "available" << endl;
				for (int j = 1; j < numR; j++) {
					cout << avail[j] << " ";
				}
				cout << endl;
				printArrs(allocs, maxRes, calcNeed(allocs, maxRes));
				cout << "\n////////////////////// -               End              - //////////////////////" << endl;
			}
			vector<vector<int>> pAllocs{ allocs[i] };
			vector<vector<int>> pMax{ maxRes[i] };
			vector<vector<int>> pNeed = calcNeed(pAllocs, pMax);
			cout << "\nP#" << i << " requests allocaiton\n";
			cout << "\n////////////////////// - Process State - //////////////////////" << endl;
			cout << endl << "available" << endl;
			for (int j = 1; j < numR; j++) {
				cout << avail[j] << " ";
			}
			cout << endl;
			printArrs(pAllocs, pMax, pNeed);
			cout << "\n////////////////////// -      End      - //////////////////////" << endl;

			vector<int> request(numR, 0);
			vector<vector<int>> testAllocs = allocs;
			vector<int> testAvailable = avail;
			cout << endl << "Request" << endl;
			for (int j = 1; j < numR; j++) {
				request[j] = rand() % (pNeed[0][j] + 1);
				testAvailable[j] -= request[j];
				testAllocs[i][j] += request[j];
				cout << request[j] << " ";
			}
			cout << endl;
			bool zeroNeed = true;
			for (int j = 1; j < numR; j++) {
				zeroNeed = zeroNeed && (pNeed[0][j] == 0);
			}
			if (zeroNeed) {
				cout << "\nPROCESS NEEDS ZERO RESOURCES\n";
			}
			else if (isSafe(testAllocs, maxRes, testAvailable)) {
				cout << "\nPROCESS GRANTED ALLOCATION\n";
				avail = testAvailable;
				allocs = testAllocs;
			}
			else {
				cout << "\nPROCESS NOT GRANTED ALLOCATION, SYSTEM WOULD BE UNSAFE\n";
			}
			cout << "\n////////////////////// - After Process testing - //////////////////////" << endl;
			cout << endl << "available" << endl;
			for (int j = 1; j < numR; j++) {
				cout << avail[j] << " ";
			}
			cout << endl;
			printArrs(allocs, maxRes, calcNeed(allocs, maxRes));
			cout << "\n////////////////////// -           End          - //////////////////////" << endl;
		}
	}

	cout << "\n////////////////////// - Final State of the System - //////////////////////" << endl;
	cout << endl << "available" << endl;
	for (int j = 1; j < numR; j++) {
		cout << avail[j] << " ";
	}
	cout << endl;
	printArrs(allocs, maxRes, calcNeed(allocs, maxRes));
	cout << "\n////////////////////// -           End             - //////////////////////" << endl;


}

int main() {
	Banker();
}
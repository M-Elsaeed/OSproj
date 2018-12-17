#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
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
		disallocs[i] = r;
	}
	cout << endl;
	for (int i = 1; i < available.size(); i++) {
		available[i] += disallocs[i];
		allocated[pNum][i] -= disallocs[i];
	}
	cout << endl << "randomly disallocated P#" << allocated[pNum][0] << endl;
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
			allocs[i][0] = i;
			for (int j = 1; j < numR; j++) {
				cin >> maxRes[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < numP; i++) {
			maxRes[i][0] = i;
			allocs[i][0] = i;
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

int arrSearch(vector<int> arr, int val) {
	auto it = find(arr.begin(), arr.end(), val);
	if (it != arr.end()) {
		return (it - arr.begin());
	}
	return -1;
}


int FIFO(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	queue <int> Q;
	// Up to the buffer's size misses are counted assuming the buffer is initially empty
	for (int i = 0; i < buffer.size(); i++) {
		buffer[i] = refs[i];
		miss++;
		Q.push(refs[i]);
	}
	for (int i = buffer.size(); i < refs.size(); i++) {
		int index = arrSearch(buffer, refs[i]);
		// not found
		if (index < 0) {
			miss++;
			buffer[arrSearch(buffer, Q.front())] = refs[i];
			Q.pop();
			Q.push(refs[i]);
		}
	}
	return miss;
}

int LRU(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	vector<int>lastUsed;
	// Up to the buffer's size misses are counted assuming the buffer is initially empty
	for (int i = 0; i < buffer.size(); i++) {
		buffer[i] = refs[i];
		miss++;
		// if page already referenced before bring it to the top of the vector lastUsed
		// else just pushback
		int ru = arrSearch(lastUsed, refs[i]);
		if (ru >= 0) {
			lastUsed.erase(lastUsed.begin() + ru);
		}
		lastUsed.push_back(refs[i]);
	}
	for (int i = buffer.size(); i < refs.size(); i++) {
		if (lastUsed.size() > buffer.size()) {
			for (int j = 0; j < lastUsed.size() - buffer.size(); j++) {
				lastUsed.erase(lastUsed.begin() + j);

			}
		}
		if (arrSearch(buffer, refs[i]) < 0) {
			buffer[arrSearch(buffer, lastUsed[0])] = refs[i];
			miss++;
		}
		// if page already referenced before bring it to the top of the vector lastUsed
		// else just pushback
		int ru = arrSearch(lastUsed, refs[i]);
		if (ru >= 0) {
			lastUsed.erase(lastUsed.begin() + ru);
		}
		lastUsed.push_back(refs[i]);

	}
	return miss;
}

int LFU(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	vector <int> freq (100,0);
	// Up to the buffer's size misses are counted assuming the buffer is initially empty
	for (int i = 0; i < buffer.size(); i++) {
		buffer[i] = refs[i];
		freq[refs[i]]++;
		miss++;
	}
	for (int i = buffer.size(); i < refs.size(); i++) {
		int index = arrSearch(buffer, refs[i]);
		// not found
		if (index < 0) {
			miss++;
			int buffInd = 0;
			for (int j = 0; j < buffer.size(); j++) {
				buffInd = freq[buffer[j]] < freq[buffer[buffInd]] ? j : buffInd;
			}
			buffer[buffInd] = refs[i];
		}
		freq[refs[i]]++;
	}
	return miss;
}

int secondChance(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	vector<int>refBits(buffer.size(), 0);
	// Up to the buffer's size misses are counted assuming the buffer is initially empty
	for (int i = 0; i < buffer.size(); i++) {
		buffer[i] = refs[i];
		refBits[i] = 1;
		miss++;
	}
	for (int i = buffer.size(); i < refs.size(); i++) {
		int index = arrSearch(buffer, refs[i]);
		if (index < 0) {
			miss++;
			for (int j = 0; j < refBits.size(); j++) {
				if (refBits[j] == 0) {
					buffer[j] = refs[i];
					refBits[j] = 1;
					break;
				}
				else //if (refBits[j] == 1)
				{
					refBits[j]--;
				}
				if (j == buffer.size() - 1)
					j = -1;
			}

		}
		else {
			refBits[index] = 1;
		}

	}
	return miss;
}

int enhancedSecondChance(vector <int>refs, vector <int>buffer) {
	return 0;
}

int optimal(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	// Up to the buffer's size misses are counted assuming the buffer is initially empty
	for (int i = 0; i < buffer.size(); i++) {
		buffer[i] = refs[i];
		miss++;
	}
	for (int i = buffer.size(); i < refs.size(); i++) {
		int index = arrSearch(buffer, refs[i]);
		if (index < 0) {
			miss++;
			int buffInd = 0;
			int refsInd = i + 1;
			for (int j = 0; j < buffer.size(); j++) {
				int k;
				for (k = i + 1; k < refs.size(); k++) {
					if (buffer[j] == refs[k])
					{
						if (k > refsInd)
						{
							refsInd = k;
							buffInd = j;
						}
						break;
					}
				}
				if (k == refs.size()) {
					refsInd = k;
					buffInd = j;
				}
			}
			buffer[buffInd] = refs[i];
		}

	}
	return miss;
}

void Memmory() {
	vector <int>refs{ 7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1 };
	vector<int>buffer{ -1,-1,-1 };//(rand() % (20) + 1, -1);
	//cout << "Refereced Pages " << endl;
	//cout << "======" << endl;
	//for (int i = 0; i < 100; i++) {
	//	refs.push_back(rand() % 100);
	//	if (refs[i] > 9) {
	//		cout << "| " << refs[i] << " |" << endl;
	//	}
	//	else {
	//		cout << "| " << refs[i] << "  |" << endl;
	//	}
	//}
	//cout << "======" << endl;
	cout << endl << "FIFO : " << FIFO(refs, buffer) << endl;//Verified
	cout << endl << "LRU : " << LRU(refs, buffer) << endl;//Verified
	cout << endl << "LFU : " << LFU(refs, buffer) << endl;//Verify
	cout << endl << "secondChance : " << secondChance(refs, buffer) << endl;//Verified
	cout << endl << "enhancedSecondChance : " << enhancedSecondChance(refs, buffer) << endl;
	cout << endl << "optimal : " << optimal(refs, buffer) << endl;//Verified
}

int main() {
	Memmory();
}
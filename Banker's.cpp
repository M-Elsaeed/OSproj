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
	for (int i = 0; i < refs.size(); i++) {
		int empty = arrSearch(buffer, -1);
		int index = arrSearch(buffer, refs[i]);
		if (empty >= 0 && index < 0) {
			buffer[empty] = refs[i];
			miss++;
			Q.push(refs[i]);
		}
		// not found
		else if (index < 0) {
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

	for (int i = 0; i < refs.size(); i++) {
		if (lastUsed.size() > buffer.size()) {
			for (int j = 0; j < lastUsed.size() - buffer.size(); j++) {
				lastUsed.erase(lastUsed.begin() + j);

			}
		}
		int empty = arrSearch(buffer, -1);
		int index = arrSearch(buffer, refs[i]);
		if (empty >= 0 && index < 0) {
			buffer[empty] = refs[i];
			miss++;
		}
		else if (index < 0) {
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
	vector <int> freq(100, 0);
	for (int i = 0; i < refs.size(); i++) {
		int index = arrSearch(buffer, refs[i]);
		int empty = arrSearch(buffer, -1);
		if (empty >= 0 && index < 0) {
			miss++;
			buffer[empty] = refs[i];
		}
		else if (index < 0) {
			miss++;
			int buffInd = 0;
			for (int j = 0; j < buffer.size(); j++) {
				buffInd = freq[buffer[j]] < freq[buffer[buffInd]] ? j : buffInd;
			}
			buffer[buffInd] = refs[i];//resetting counter after removing?
			freq[buffer[buffInd]] = 0;
		}
		freq[refs[i]]++;
		cout << endl << "/////////////////////" << endl << "Mem : ";
		for (int m = 0; m < buffer.size(); m++)
			cout << buffer[m] << " ";
		cout << endl << "Num : ";
		for (int m = 0; m < freq.size(); m++)
			cout << m << " ";
		cout << endl << "frq : ";
		for (int m = 0; m < freq.size(); m++)
			cout << freq[m] << " ";
		cout << endl << "/////////////////////" << endl;
	}
	return miss;
}

int secondChance(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	vector<int>refBits(buffer.size(), 0);
	vector <int> Q;
	for (int i = 0; i < refs.size(); i++) {
		cout << "refeering" << refs[i] << endl;
		int empty = arrSearch(buffer, -1);
		int index = arrSearch(buffer, refs[i]);
		if (empty >= 0 && index < 0) {
			miss++;
			buffer[empty] = refs[i];
			refBits[empty] = 0;
			//refBits[empty] = 1;
			Q.push_back(refs[i]);
		}
		else if (index < 0) {
			miss++;
			while (true) {
				int nxtVictim = arrSearch(buffer, Q[0]);
				if (refBits[nxtVictim] == 0) {
					while (arrSearch(Q, buffer[nxtVictim]) >= 0) {
						Q.erase(Q.begin() + arrSearch(Q, buffer[nxtVictim]));
					}
					Q.push_back(refs[i]);
					buffer[nxtVictim] = refs[i];
					refBits[nxtVictim] = 0;
					//refBits[nxtVictim] = 1;
					break;
				}
				else //if (refBits[j] == 1)
				{
					refBits[nxtVictim] = 0;
					while (arrSearch(Q, buffer[nxtVictim]) >= 0) {
						Q.erase(Q.begin() + arrSearch(Q, buffer[nxtVictim]));
					}
					Q.push_back(buffer[nxtVictim]);
				}
			}
		}
		else {
			refBits[index] = 1;
		}

		cout << endl << "/////////////////////" << endl << "Que : ";
		for (int m = 0; m < Q.size(); m++)
			cout << Q[m] << " ";
		cout << endl << endl << endl << "Mem : ";
		for (int m = 0; m < buffer.size(); m++)
			cout << buffer[m] << " ";
		cout << endl << "Ref : ";
		for (int m = 0; m < refBits.size(); m++)
			cout << refBits[m] << " ";
		cout << endl << "/////////////////////" << endl;

	}
	return miss;
}

int enhancedSecondChance(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	vector<int>refBits(buffer.size(), 0);
	vector<int>modBits(buffer.size(), 0);
	// 00, 01, 10, 11
	vector <int> Q;
	int nxtVictim = 0;
	for (int i = 0; i < refs.size(); i++) {
		int modding = rand() % 2;
		cout << endl << "referencing : " << refs[i] << "  ";
		if (modding) {
			cout << " MODIFYING" << endl;

		}
		else {
			cout << " READ ONLY" << endl;
		}
		int index = arrSearch(buffer, refs[i]);
		int empty = arrSearch(buffer, -1);
		if (empty >= 0 && index < 0) {
			miss++;
			buffer[empty] = refs[i];
			//Q.push_back(refs[i]);
			refBits[empty] = 0;
			//refBits[empty] = 1;
			modBits[empty] = (i == 0 || i == 9 || i == 16 || i == 2 || i == 10 || i == 6 || i == 8 || i == 13) ? 1 : 0;//modding;
			nxtVictim = empty + 1 == buffer.size() ? 0 : empty + 1;
		}
		else if (index < 0) {
			miss++;
			//bool found = false;
			//while (!found) {
			//	// Q.size();
			bool reset = false, found = false;
			while (nxtVictim < buffer.size()) {
				if (refBits[nxtVictim] == 0 && modBits[nxtVictim] == 0) {
					//while (arrSearch(Q, buffer[nxtVictim]) >= 0) {
					//	Q.erase(Q.begin() + arrSearch(Q, buffer[nxtVictim]));
					//}
					//Q.push_back(refs[i]);
					buffer[nxtVictim] = refs[i];
					refBits[nxtVictim] = 0;
					//refBits[nxtVictim] = 1;
					modBits[nxtVictim] = (i == 0 || i == 9 || i == 16 || i == 2 || i == 10 || i == 6 || i == 8 || i == 13) ? 1 : 0;//modding;
					found = true;
					nxtVictim = nxtVictim + 1 == buffer.size() ? 0 : nxtVictim + 1;
					break;
				}
				if (!reset) {
					nxtVictim = nxtVictim + 1 == buffer.size() ? 0 : nxtVictim + 1;
					reset = true;
				}
				else {
					nxtVictim++;
				}
			}
			reset = false;
			if ((!found) && nxtVictim == buffer.size())
				nxtVictim = 0;
			while ((!found) && nxtVictim < buffer.size()) {

				if (refBits[nxtVictim] == 0 && modBits[nxtVictim] == 1) {
					//while (arrSearch(Q, buffer[nxtVictim]) >= 0) {
					//	Q.erase(Q.begin() + arrSearch(Q, buffer[nxtVictim]));
					//}
					//Q.push_back(refs[i]);
					buffer[nxtVictim] = refs[i];
					refBits[nxtVictim] = 0;
					//refBits[nxtVictim] = 1;
					modBits[nxtVictim] = (i == 0 || i == 9 || i == 16 || i == 2 || i == 10 || i == 6 || i == 8 || i == 13) ? 1 : 0;//modding;
					found = true;
					nxtVictim = nxtVictim + 1 == buffer.size() ? 0 : nxtVictim + 1;
					break;
				}
				else if (refBits[nxtVictim] == 1)
				{
					refBits[nxtVictim] = 0;
				}
				if (!reset) {
					nxtVictim = nxtVictim + 1 == buffer.size() ? 0 : nxtVictim + 1;
					reset = true;
				}
				else {
					nxtVictim++;
				}

			}

			//bool all01 = true;
			//for (int x = 0; x < x < buffer.size(); x++) {
			//	all01 = all01 && (refBits[x] == 0) && (modBits[x] == 0);
			//}

		}
		else {
			refBits[index] = 1;
		}
		cout << endl << "/////////////////////" << endl << "Mem : ";
		for (int m = 0; m < buffer.size(); m++)
			cout << buffer[m] << " ";
		cout << endl << "Ref : ";
		for (int m = 0; m < refBits.size(); m++)
			cout << refBits[m] << " ";
		cout << endl << "Mod : ";
		for (int m = 0; m < refBits.size(); m++)
			cout << modBits[m] << " ";
		cout << endl << "/////////////////////" << endl;
	}
	return miss;
}

int optimal(vector <int>refs, vector <int>buffer) {
	int miss = 0;
	// Up to the buffer's size misses are counted assuming the buffer is initially empty
	for (int i = 0; i < refs.size(); i++) {
		int index = arrSearch(buffer, refs[i]);
		int empty = arrSearch(buffer, -1);
		if (empty >= 0 && index < 0) {
			buffer[empty] = refs[i];
			miss++;
		}
		else if (index < 0) {
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
	vector <int>refs{ 0,1,3,6,2,4,5,2,5,0,3,1,2,5,4,1,0 };//{ 1,2,3,4,1,3,6,2,1,5,3,7,6,3,2,1,2,3,4,6 };//{ 7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1 };//{ 2,3,2,1,5,2,4,5,3,2,5,2 };
	vector<int>buffer{ -1,-1,-1,-1 };//(rand() % (20) + 1, -1);
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
	//cout << endl << "FIFO : " << FIFO(refs, buffer) << endl;//Verified
	//cout << endl << "LRU : " << LRU(refs, buffer) << endl;//Verified
	//cout << endl << "LFU : " << LFU(refs, buffer) << endl;//Verify
	//cout << endl << "secondChance : " << secondChance(refs, buffer) << endl;//Verified
	cout << endl << "enhancedSecondChance : " << enhancedSecondChance(refs, buffer) << endl;
	//cout << endl << "optimal : " << optimal(refs, buffer) << endl;//Verified
}

int main() {
	Memmory();
}



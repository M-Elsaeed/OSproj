#include <iostream>
#include <vector>
#include <tuple>
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
	cout << "/////////// Side By Side ////////////" << endl;
	cout << "// Allocated || Max || Needed //" << endl;
	for (int i = 0; i < allocs.size; i++) {
		for (int j = 0; j < allocs[i].size; j++) {
			cout << allocs[i][j] << " ";
		}
		cout << "|| ";
		for (int j = 1; j < allocs[i].size; j++) {

			cout << maxRes[i][j] << " ";
		}
		cout << "|| ";
		for (int j = 1; j < allocs[i].size; j++) {

			cout << needed[i][j] << " ";
		}
		cout << endl;

	}
}


tuple<bool, vector<vector<int>>> isSafe(vector<vector<int>> allocated, vector<vector<int>> maximum, vector<int> available)
{
	// Calculation of needed matrix
	vector<vector<int>> need = calcNeed(allocated, maximum);
	vector<vector<int>> temp = need;
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
			cout << endl << "Executed P#" << allocated[i][0] << endl;
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
	return make_tuple(safe, temp);
}

void project() {
	int numP, numR, userMode;
	cout << "Enter 0 for random numbers and 1 for User Input" << endl;
	cin >> userMode;
	cout << "Enter number of processes" << endl;
	cin >> numP;
	cout << "Enter number of resources" << endl;
	cin >> numR;
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
			cout << "Enter number of allocated resources for process#" << i << endl;
			allocs[i][0] = i;
			for (int j = 1; j < numR; j++) {
				cin >> allocs[i][j];
			}
		}

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
		for (int i = 0; i < numP; i++) {
			allocs[i][0] = i;
			for (int j = 1; j < numR; j++) {
				int x = rand() % (maxRes[i][j] + 1);
				allocs[i][j] = x;
			}
		}
	}
	printArrs(allocs, maxRes, calcNeed(allocs, maxRes));
	vector<int> AVAILABLE = avail;
	vector<vector<int>> ACCEPTED(numP, vector<int>(numR, 0));
	vector<vector<int>> ALLOCATIONS(numP, vector<int>(numR, 0));
	vector<vector<int>> MAX_RESOURCES(numP, vector<int>(numR, 0));
	for (int i = 0; i < numP; i++) {
		cout << "P#" << i << " requests allocaiton\n";
		vector<vector<int>> mockAllocs{ allocs[i] };
		vector<vector<int>> mockMax{ maxRes[i] };
		auto tempNeed = calcNeed(mockAllocs, mockMax);
		printArrs(mockAllocs, mockMax, tempNeed);



		bool safe;

		vector<vector<int>> vec;
		tie(safe, vec) = isSafe(allocs, maxRes, avail);
	}


}






int main() {
	project();
	cout << "x";
}
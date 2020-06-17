#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>

using namespace std;

//Time Complexity = nlogn due to priority queue (max heap), where n is the number of racks, 
//simply get p/L ratio and sort in descending order to get arrangement then calculate expected time 

int main(int argc, char** argv)
{
	fstream inFile;
	string t;
	int n;
	double x;
	
	inFile.open(argv[1]);
	inFile >> t >> n; 
	
	vector<double> L;
    vector<double> p;

	inFile >> t;
	for (int j = 0; j < n; j++){
		inFile >> x;
		L.push_back(x);
	}
	inFile >> t;
	for (int j = 0; j < n; j++){
		inFile >> x;
		p.push_back(x);
	}
	inFile.close();

	priority_queue <pair<double,int>> maxq; //where each pair is a rack, double is the p/L ratio, and int is the rack number (0..n-1)
	for (int r = 0; r < n; r++){
		maxq.push(make_pair(p[r]/L[r], r));
	}

	string out = "";
	double expTime = 0;
	double currLengthSum = 0;
	for (int r = 0; r < n; r++){
		int rack = maxq.top().second;
		currLengthSum += L[rack];
		expTime += p[rack]*currLengthSum;

		out += to_string(rack+1); // rack numbers must be 1 .. n (so +1)
		if (r != n-1){ // do not add space at the end
			out += " ";
		}
		maxq.pop();
	}
	out += "\nExpected Time " + to_string(expTime) + '\n';
	cout << out;
	
	return 0;
}
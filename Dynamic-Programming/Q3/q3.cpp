#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <queue>
using namespace std; 
  
vector<int> m;
vector<int> ordering;
int **memoArr;
int **costCutArr;


int leastCost(int starti, int endi){ 
	// cout <<  starti << " ===== " << endi << endl;
	if(memoArr[starti][endi] != INT32_MAX){		
		return memoArr[starti][endi];
	}

	int currLen = endi - starti; //current stick length
	bool iscut = false;
	int leastCutCost = INT32_MAX;
	int leastCut = m[0]; 
	for(int cutpos : m){ //for every cutpos
		if(cutpos > starti && cutpos < endi){ //valid for this piece
			// cout << "cutting at " << cutpos << endl;
			iscut = true;
			int cost = leastCost(starti, cutpos) + leastCost(cutpos, endi) + currLen;
			if (cost < leastCutCost){
				leastCutCost = cost;
				leastCut = cutpos;
			}
		}
	}

	if(!iscut) leastCutCost = 0; //the cuts could not be applied to this piece, cost == 0
    else{
		costCutArr[starti][endi] = leastCut;
	}
	memoArr[starti][endi] = leastCutCost;
	return memoArr[starti][endi];
}
	
void setOrdering(int starti, int endi){ //going though the memoization grid to trace back where the min came from 
	if (costCutArr[starti][endi] == 0) //not cut at all
		return;
	int cutpos = costCutArr[starti][endi];
	int leftpieceCut = costCutArr[starti][cutpos];
	int rightpieceCut = costCutArr[cutpos][endi];
	if (leftpieceCut != 0) //not cut at all
		ordering.push_back(leftpieceCut);
	if (rightpieceCut != 0)
		ordering.push_back(rightpieceCut);
	setOrdering(starti, cutpos); //left piece
	setOrdering(cutpos, endi); //right piece
}

int main(int argc, char** argv) 
{ 
	ifstream inFile;
	istringstream ss;
	char t;
	int n, x;
	string l;
	
	inFile.open(argv[1]);
	inFile >> t >> n; //ruler size
	getline(inFile, l);

	getline(inFile, l);
	ss.str(l);
	ss >> t;
	while (!ss.eof()){ // until end of the stream (line)
		ss >> x;
		m.push_back(x); //cut positions
	}
	inFile.close();

	// n+1 x n+1 dynamic array formed
	memoArr = new int*[n+1];
	costCutArr = new int*[n+1]();
	for (int i = 0; i < n+1; i++){
		memoArr[i] = new int[n+1];
		costCutArr[i] = new int[n+1]();
	}
	
	for (int i = 0; i < n+1; i++){
		for (int j = 0; j < n+1; j++){
			memoArr[i][j] = INT32_MAX;
		}	
	}

    int res = leastCost(0, n);
	ordering.push_back(costCutArr[0][n]);
	setOrdering(0, n);

	string ans = "Optimal cut ordering: ";
	for (int i = 0; i < ordering.size(); i++){
		ans += to_string(ordering[i]);
		if (i != m.size()-1)
			ans += " ";
	}
	ans += "\nLeast cost: " + to_string(res);
   	cout<<ans<<endl;
    return 0; 
} 

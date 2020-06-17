#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>

using namespace std;
struct T;

struct L{
	int ID;
	T* pair;
	L(int i){
		ID = i;
		pair = NULL;
	}
};

struct T{
	int ID;
	bool free;
	vector<L*> prefList;
};

int charToInt(char c){
	return int(c)-48;
}

int main(int argc, char** argv)
{
	//INITIALIZATION OF GRAPH FROM INPUT FILE
	ifstream inFile;
	istringstream ss;
	char t;
	int n, x, i, j;
	string l, s;
	
	inFile.open(argv[1]);
	inFile >> t >> n;
	getline(inFile, l);
	
    vector<T> teams(n);
	vector<L> locations;
	for (i = 0; i < n; i++){
		locations.push_back(L(i+1));
	}
    vector<vector<int>> inputLists(n);
	// 1 0 2 0 3 0
	// 0 1 0 2 0 3
	// 2 0 3 0 1 0

	vector<vector<int>> LprefMatrix(n);
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			LprefMatrix[i].push_back(0);
		}
	} //nxn matrix initialized

	for (i = 0; i < n; i++){		
		getline(inFile, l);
		ss.str(l); // Make an input stringstream to read from the line's string 
		ss >> t >> x >> t; // where token == ':'
		teams[i].ID = x;
		teams[i].free = true;
		
		while (!ss.eof()){ // until end of the stream (line), keep reading in vertices (IDs)
			ss >> s;
			if (s.length() > 1){ // not a -
				x = charToInt(s[1]);
				teams[i].prefList.push_back(&locations[x-1]);
				inputLists[i].push_back(x);
			}
			else if (s.length() == 1){
				inputLists[i].push_back(0); //0 for dash
			}
			s = "";
		}
		ss.clear(); //Clear for next iteration
	}
	
	for (j = inputLists[0].size()-1; j >= 0; j--){
		for (i = 0; i < n; i++){
			x = inputLists[i][j];
			if (x != 0){
				LprefMatrix[i][x-1] = j;
			}
		}
	}
	/*
	   L1 L2 L3
	T1 0  2  4
	T2 1  3  5
	T3 4  0  2

	L1 prefers T3 the more than T2 since LprefMatrix[2][0] > LprefMatrix[1][0]
	*/
	
	queue<T*> freeTeams;
	for (i = 0; i < n; i++){ //initialization, push all teams as free
		freeTeams.push(&teams[i]);
	}

	while(!freeTeams.empty()){
		T* currT = freeTeams.front();
		for (L* currL : currT->prefList){ 
			if (currL->pair == NULL){
				currL->pair = currT; //(L,T) pair formed
				freeTeams.pop(); // T no longer free
				break;
			}
			else{ // some (L,T) pair already exists
				if (LprefMatrix[currT->ID-1][currL->ID-1] > LprefMatrix[currL->pair->ID-1][currL->ID-1]){
					T* Tdash = currL->pair;
					currL->pair = currT;
					freeTeams.pop(); //remove T from freeTeams
					freeTeams.push(Tdash); // push T' to freeTeams again, since pair broken			
					break;
				}
				//else no change				
			}
		}
	}
	
	// int freeCount = n;
	// while (freeCount > 0) 
    // { 
	// 	T* currT = NULL; 
	// 	for (i = 0; i < n; i++){
	// 		if (teams[i].free == true){
	// 			currT = &teams[i];
	// 			break;
	// 		}
	// 	}
	// 	// cout << "T:" << currT->ID << endl;		
    //     for (i = 0; i < n && currT->free == true; i++){ 
	// 		L* currL = currT->prefList[i];
    //     	// cout << "L:" << currL->ID << endl;

    //         if (currL->pair == NULL) 
    //         { 
    //             currL->pair = currT; 
    //             currT->free = false; 
    //             freeCount--; 
    //         } 
    //         else 
    //         { 
    //             T* currPair = currL->pair;

    //         	if (LprefMatrix[currT->ID-1][currL->ID-1] > LprefMatrix[currPair->ID-1][currL->ID-1]) 
    //             { 
	// 				currL->pair = currT; 
	// 				currT->free = false;
	// 				currPair->free = true;
    //             } 
    //         } 
	// 		// cout << endl;
    //     }
    // } 

	string out = "Final Destinations: ";
	for (i = 0; i < n; i++){
		out += 'L' + to_string(i+1) + " T" + to_string(locations[i].pair->ID);
		if (i != n-1)
			out += ", ";
	}
	cout << out;
	return 0;
}
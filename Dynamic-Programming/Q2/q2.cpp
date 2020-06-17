#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
using namespace std;


int minLights(int i, int parent);

int* memoArr; //initially all 0
vector<list<int>> adjList;
vector<int> litVertices;

int minLights(int i, int parent) {
	if (adjList[i].size() == 0) // dead end
        return 0; 
	if (adjList[i].size() == 1 ) //only one node ahead e.g. 0 : 3
		return 0;
	
	// already evaluated then do not further do so
    if (memoArr[i] != -1) 
        return memoArr[i]; 

    // lights required with root 
    int lightsRoot = 1;
	list<int>::iterator itr, itrChild;
	for (itr = adjList[i].begin(); itr != adjList[i].end(); ++itr){
		int destv = (*itr);
		if (destv != parent){ // to prevent inf loop
			lightsRoot += minLights(destv, i);
		}
	}
	
    // lights required without root 
    int lightsNoRoot = 0; 
	for (itr = adjList[i].begin(); itr != adjList[i].end(); ++itr){
		int destv = (*itr); //check children for every child vertex of root
		if (destv != parent){ // to prevent inf loop
			lightsNoRoot += 1;
			for (itrChild = adjList[destv].begin(); itrChild != adjList[destv].end(); ++itrChild){
				int childOfChild = (*itrChild);
				if (childOfChild != i){ 
					lightsNoRoot += minLights(childOfChild, destv);
				}
			}
		}	
	} 

    memoArr[i] = min(lightsNoRoot, lightsRoot); //prefer the option that needs less lights, min of the two
	if (memoArr[i] == lightsRoot) //withroot was chosen
		litVertices.push_back(i);

    return memoArr[i]; 
}   
  
int main(int argc, char** argv) 
{ 
	ifstream inFile;
	istringstream ss;
	char token;
	int noVertices, srcid, destid;
	string l;
	
	inFile.open(argv[1]);
	inFile >> token >> noVertices;
	
	getline(inFile, l);

    for (int i = 0; i < noVertices; i++){
		list<int> newList;
		adjList.push_back(newList);
    }

	for (int i = 0; i < noVertices; i++){		
		getline(inFile, l);
		ss.str(l); // Make an input stringstream to read from the line's string 
		ss >> srcid >> token; // where token == ':'
		if (ss.str().length()==3)
			continue;
		while (!ss.eof()){ // until end of the stream (line), keep reading in vertices (IDs)
			ss >> destid;
		    adjList[srcid].push_back(destid);
		}
		ss.clear(); //Clear for next iteration
	}
	inFile.close();

	memoArr = new int[noVertices];
    for (int i = 0; i < noVertices; i++){
		memoArr[i] = -1;
	}

	// for (int i = 0; i < noVertices; i++){
	// 	cout << i << " : ";
	// 	list<int>::iterator itr = adjList[i].begin();
	// 	while (itr != adjList[i].end()) {
	// 		int dest = *itr; //dest == neighbor destination vertex connected to v
	// 		cout << dest << " ";
	// 		itr++;	
	// 	}
	// 	cout << endl;
	// }


	int res = minLights(0, -1); //no parent initially of root
	string ans = "Minimum Lights ";
	ans += to_string(litVertices.size());
	ans += "\n{";
	for (int litV : litVertices){
		ans += to_string(litV) + ", ";
	}
	
	// for (int i = 0; i < noVertices; i++){
	// 	cout << memoArr[i] << " ";
	// }

	ans = ans.substr(0, ans.length()-2) + "}";
	cout << ans; 
	delete[] memoArr;

    return 0; 
}
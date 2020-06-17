#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

struct Vertex
{
	int ID;
	int inDeg;

	Vertex(int i){
		ID = i;
		inDeg = 0;
	}
};

string convAdjListStr(vector<list<Vertex*>> adjList){
	string result = "";
	for (int j = 0; j <adjList.size(); j++){
        list<Vertex*>::iterator itr = adjList[j].begin();
        
        while (itr != adjList[j].end()) {
            result += to_string ((*itr)->ID) + ' ';
		    itr++;
        }
		result = result.substr(0,result.length()-1); //remove the last space
		result += '\n';
	}
	return result;
}

//Kahn's Algorithm with topSort
int topSort(vector<list<Vertex*>> adjList, vector<list<Vertex*>> &resAL) { //result as adjacency list for the format of rows
	queue<Vertex*> topQueue;
	for (int i = 0; i < adjList.size(); i++){ //enqueue vertices if their indegree is 0 (no incoming edges, only possible outgoing)
		Vertex* v = adjList[i].front();
		if (v->inDeg == 0){
			resAL[0].push_back(v); //add to row 0 initially
			topQueue.push(v);
		} 
	}

	int R = 1;
	while (!topQueue.empty()) { //until all vertices have had their in degrees updated
		// all incoming edges removed as in top sort
		Vertex* v = topQueue.front();
		topQueue.pop(); //remove vertex after updating and push it to the result	
		// keep recursing for all adjacent vertices of the front of the queue dequeued v
		list<Vertex*>::iterator itr = adjList[v->ID].begin();
		itr++; //start from second element
		//for all neighboring nodes
		while (itr != adjList[v->ID].end()) {
			Vertex* dest = *itr; //dest == neighbor destination vertex connected to v
			dest->inDeg--;
			if (dest->inDeg == 0){ //if dest's indegree becomes 0 move it to the queue
				topQueue.push(dest);
				resAL[R].push_back(dest);
			}
			itr++;	
		}
		R++;
	}

	//count the actual number of non-zero rows in the adjList
	int noRows = 0;
	for (int i = 0; i < resAL.size(); i++){
		if (resAL[i].size()!=0){
			noRows++;
		}
	}
	return noRows;
}


int main(int argc, char** argv)
{
	/*
	O(V+E) Algorithm, using Kahn's algorith with topological sort, row-wise checking vertices using queues
	and updating their in-degrees until they become 0. If nothing is added row-wise, then there was a cycle.
	*/
	ifstream inFile;
	istringstream ss;
	char token;
	int noVertices, srcid, destid;
	string l;
	
	inFile.open(argv[1]);
	inFile >> token >> noVertices;
	getline(inFile, l);
	
    vector<list<Vertex*>> adjList(noVertices);
    for (int i = 0; i < noVertices; i++){
        adjList[i].push_back(new Vertex(i));
    }

	for (int i = 0; i < noVertices; i++){		
		getline(inFile, l);
		ss.str(l); // Make an input stringstream to read from the line's string 
		ss >> srcid >> token; // where token == ':'
		if (ss.str().length()==3) //for vertices without any outgoing edges (dead end)
			continue;
		while (!ss.eof()){ // until end of the stream (line), keep reading in vertices (IDs)
			ss >> destid;
		    adjList[srcid].push_back(adjList[destid].front());
		}
		ss.clear(); //Clear for next iteration
	}
	
	inFile.close();

	stack<Vertex*> topOrder; //for result
	
	//update indegrees initially - O(V+E) step
	for (int j = 0; j <noVertices; j++){
        list<Vertex*>::iterator itr = adjList[j].begin();
        itr++; //starting from the second element
		while (itr != adjList[j].end()) {
			(*itr)->inDeg++;
			itr++;
        }
	}
	
	string x = "";
	vector<list<Vertex*>> resAL(noVertices);
	int R = topSort(adjList,resAL);
	bool emptyResult = false; //cyclic
	if (resAL[0].size() == 0){
		x="No\n";
	}
	else{
		x="Yes\n";
		x+="R "+to_string(R)+'\n';
		x+=convAdjListStr(resAL);
	}
	cout << x;
	return 0;
}


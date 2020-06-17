#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
using namespace std;

struct Vertex
{
	int ID;
	bool visit;
	bool inRecursion;
	Vertex* parent;

	Vertex(int i){
		ID = i;
		visit = false;
		inRecursion = false;
	}
};


void topSort(Vertex* v, vector<list<Vertex*>> adjList, stack<Vertex*> &topOrder, Vertex* parent, bool &isNotCyclic) { 
	v->visit = true;
	v->inRecursion = true;
	v->parent = parent; //mark in recursion stack
	// recur for all adjacent vertices
	list<Vertex*>::iterator itr = adjList[v->ID].begin();
	itr++; //start from second element
	while (itr != adjList[v->ID].end()) {
		Vertex* dest = *itr; //dest == neighbor destination vertex connected to v
		if (dest->visit && dest != parent && dest->inRecursion){ 
			// if adjacent vertex was already visited and is in recursion stack, then dead-end, cycle found
			// != parent condition ensures that we did not take another route but it is really a cycle
			isNotCyclic = false;

			//tried but didn't work :(
			//Vertex* backTracker = dest;
			//cout << "cycle time" << backTracker->ID;
			// while (backTracker!= NULL){
			// 	cout << backTracker->ID << " ";
			// 	backTracker = backTracker->parent;
			// }
			return;
		}
		if(!dest->visit){
			topSort(dest, adjList, topOrder, v, isNotCyclic);
		}
		itr++;	
	}

	topOrder.push(v); 
	v->inRecursion = false; //no longer in recursion stack
}


int main(int argc, char** argv)
{
	/*
	- Set of World Leaders (Vertices - L)
	- Set of ordered pairs (Li, Lj) = Directional Edges (Li holds a grudge against Lj)
	Ensure Li is not directly behind Lj (those they grudge) - safe ordering of Leaders, vertices,
	or let them know it is not possible (topological order if DAG)
	O(V+E) since DFS used for topological sort, adjacency list as data structure.
	Backtracking the cycle using a parent attribute on vertices. Cycle detection inside.
	*/
	stack<Vertex*> topOrder;
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
	
	bool isNotCyclic = true;
	for (int i = 0; i < noVertices; i++)
		if (!adjList[i].front()->visit)
			topSort(adjList[i].front(), adjList, topOrder, adjList[i].front(), isNotCyclic);

	string x = "No\n";
	if (isNotCyclic)
		x = "Yes\n";
	
	while (!topOrder.empty()){
		x += to_string(topOrder.top()->ID);
		if (topOrder.size() != 1)
			x += ' ';
		topOrder.pop();
	}

	x += '\n';
	
	cout << x;

	return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
using namespace std;

struct Vertex
{
	int ID, val, minID_reachable;
	bool visit;

	Vertex(int i){
		val = i;
		visit = false;
		ID = 0;
	}
};

struct Edge
{
	Vertex* src;
	Vertex* dest;

	Edge(Vertex* s, Vertex* d){
		src = s;
		dest = d;
	}
};

//marker is just an ID allotter as DFS traversal occurs downstream increasing the IDs, so it's like a timer
void DFS(Vertex* v, vector<list<Vertex*>> adjList, vector<Edge> &bridges, Vertex* parent, int marker = 0) { 
	v->visit = true;  
	v->ID = v->minID_reachable = marker;
    marker++; 
	// recur for all adjacent vertices
	list<Vertex*>::iterator itr = adjList[v->val].begin();
	itr++; //start from second element
	while (itr != adjList[v->val].end()) {
		Vertex* dest = *itr; //dest == neighbor destination vertex connected to v
		if (parent != NULL && dest->val == parent->val){ //skip the adjacent vertex that is your parent
			itr++;
			continue; // so we do not misintepret this vertex to be part of any bridges
		}
		if(!dest->visit){ //for all unvisited vertices
			DFS(dest, adjList, bridges, v, marker); //recursive DFS same as usual O(V+E)
			if (v->ID < dest->minID_reachable){ //Bridge found where ID of vertex is < minID_reachable for the dest vertex
				bridges.push_back(Edge(v,dest)); // push back bridge
			}
		}
		v->minID_reachable = min(v->minID_reachable, dest->minID_reachable); // update minimum ID reachable if one the IDs have a shorter ID route
		itr++;	
	}
} 

int main(int argc, char** argv)
{
	/*
	O(V+E)
	Adjacency List, Vertex sruct, Edge struct to push bridges - DS
	Starting from the first vertex, run DFS, give vertices increasing ID numbers, and the minimum ID reachable from a vertex during DFS
	Bridge found where ID of vertex is < minID_reachable for the dest vertex
	An edge will be a bridge iff none of the dest vertices or their descendants have back-edge to the src and its ancestors,
	and we can check that by IDs allotted that are updating
	*/

	ifstream inFile;
	istringstream ss;
	char token;
	int noVertices, srcid, destid; //where noVertices == noCities, edges == roads
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

		if (ss.str().length()==3)
			continue;
		while (!ss.eof()){ // until end of the stream (line), keep reading in vertices (vals)
			ss >> destid;
		    adjList[srcid].push_back(adjList[destid].front());
		}
		ss.clear(); //Clear for next iteration
	}
	
	inFile.close();

	vector<Edge> bridges;
	DFS(adjList[0].front(), adjList, bridges, NULL);
	
	string x = to_string(bridges.size()) + "\n";
	for (int i = 0; i < bridges.size(); ++i)
	{
		x = x + "(" + to_string(bridges[i].src->val) + "," + to_string(bridges[i].dest->val) + ")\n"; 
	}
	cout<<x;
	
	return 0;
}

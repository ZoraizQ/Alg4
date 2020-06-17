#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>

using namespace std;

struct Vertex
{
	char color; 
	int ID;
	bool visit;

	Vertex(int i){
		ID = i;
		color = 'W';
		visit = false;
	}
};

vector<Vertex*> green;
vector<Vertex*> red;
vector<Vertex*> path;

string cycle = "";
void DFS(Vertex* v, vector<list<Vertex*>> adjList) { 
	v->visit = true;
	cycle += to_string(v->ID) + "->";
	// recur for all adjacent vertices
	list<Vertex*>::iterator itr = adjList[v->ID].begin();
	itr++; //start from second element
	while (itr != adjList[v->ID].end()) {
		Vertex* dest = *itr; //dest == neighbor destination vertex connected to v
		if(!dest->visit){
			DFS(dest, adjList);
		}
		itr++;	
	}
}


bool isBipartite(vector<list<Vertex*>> adjList, int start_ind){
    queue <Vertex*> VQ;
	Vertex* start = adjList[start_ind].front();
    start->visit = true; //start vertex is marked as visited
	VQ.push(start); // and pushed into the queue for breadth-first traversal
	start->color = 'R'; // start color is Red 'R'
	char opp_color; // opposite of current vertex under observation	
	

	while(VQ.size() != 0){
		Vertex* front = VQ.front();
		if (front->color == 'R'){
			opp_color = 'G';
			red.push_back(front);
		}
		else if (front->color == 'G'){
			opp_color = 'R';
			green.push_back(front);
		}	
		path.push_back(front);
		VQ.pop();

        list<Vertex*>::iterator itr = adjList[front->ID].begin(); 
        itr++;// start from the second, since first is the start
        while (itr != adjList[front->ID].end()) {
            //if unvisited, add edge's dest vertex to queue after it is marked as visited
            Vertex* pushVertex = *itr;
			if (front->color == pushVertex->color){
				if (adjList[pushVertex->ID].size()==1){ //if 3 is a dead end
				}
				else{
					path.push_back(pushVertex);
				}
				path.push_back(start);
				return false;
			}
			
			if (!pushVertex->visit){
				pushVertex->visit = true;				
				pushVertex->color = opp_color;
                Vertex* h = adjList[pushVertex->ID].front();
				h->color = opp_color;
				VQ.push(pushVertex);
			}
            
            itr++;
        }
	}

	return true;
}



int main(int argc, char** argv)
{	
	/* 
	Color the vertices, as you are forming the bipartite graph, into two sep colors as you go
	Vertice.Color = 'W' initially ... Outgoing Edges, Ingoing Edges
	Data structures - Vertice, Edge structs, Adj List
	Approx. O(|V| + |E|) since BFS used simply, all edges and vertices visited
	trace as long you do not encounter the same color again, then it is not bipartite
	*/
	
	//INITIALIZATION OF GRAPH FROM INPUT FILE
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
		if (ss.str().length()==3)
			continue;
		while (!ss.eof()){ // until end of the stream (line), keep reading in vertices (IDs)
			ss >> destid;
		    adjList[srcid].push_back(adjList[destid].front());
		}
		ss.clear(); //Clear for next iteration
	}
	
	// for (int i = 0; i < noVertices; i++){
	// 	cout << adjList[i].front()->ID << " : ";
	// 	list<Vertex*>::iterator itr = adjList[i].begin(); 
    //     itr++;
    //     while (itr != adjList[i].end()){
	// 		cout << (*itr)->ID << " ";
	// 		itr++;
	// 	}
	// 	cout << endl;
	// }

	inFile.close();

	string x = "";
	
	if (isBipartite(adjList, 0)){
		x = "Yes\n";
		for (int i = 0; i < red.size(); i++){
			x += to_string(red[i]->ID) + ' ';
		}
		x += '\n';
		for (int j = 0; j < green.size(); j++){
			x += to_string(green[j]->ID);
			if (j != green.size()-1)
				x += ' ';
		}
		x += '\n';
	}
	else{
		x ="No\n";
		for (int i = 0; i < path.size()-1; i++){
			path[i]->visit = false;
		}
		DFS(path[0], adjList);
		cycle += to_string(path[0]->ID);
		x += cycle + '\n';
	}

	cout<<x;
	
	return 0;
}

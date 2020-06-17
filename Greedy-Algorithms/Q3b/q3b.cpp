#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <sstream>
using namespace std; 


// Time Complexity = O(ElogV), where E is the number of edges, while V is the number of vertices, logE for insertion in priority queue
// due to the Prims MST, which dominates all O(V+E) complexities for reconstruction the MST using DFS traversal
// to find cycle in the new MST, then remove the max edge in that cycle to create the new MST
// Data structure - vertex classes with edge vector for all adjacent edges, the graph is a vector of vertices, MST is a vector of edges. 

class Edge;

class Vertex
{ 
    public:
        int ID;
        int dist;
        bool visit;
        bool inRecStack;
        bool inMST;
        vector<Edge> edges;
        Vertex* parent;

	Vertex(int i){
		ID = i;
		visit = false;
        inMST = false;
        inRecStack = false;
        dist = INT32_MAX; //INF distance initially
        parent = NULL;
	}
};

class vertDistComparator 
{ 
public: 
    int operator() (const Vertex* v1, const Vertex* v2) 
    { 
        return v1->dist > v2->dist; 
    } 
}; 


class Edge
{
    public: 
        int weight;
        Vertex* src;
        Vertex* dest;

	Edge(Vertex* s, Vertex* d, int w){
		src = s;
        dest = d;
        weight = w;
	}
};

class edgeWeightComparator 
{ 
public: 
    int operator() (const Edge& e1, const Edge& e2) 
    { 
        return e1.weight < e2.weight; 
    } 
}; 

vector<Edge> PrimsMST(vector<Vertex*> vList){
    vector<Edge> MST; // edges constituting the MST
    priority_queue<Vertex*, vector<Vertex*>, vertDistComparator> minq;
    
    Vertex* start = vList[0];
    start->dist = 0;
    minq.push(start);
    
    while(!minq.empty()){
        Vertex* u = minq.top(); //get minimum distance vertex
        minq.pop();
        u->inMST = true; //using inMST to check if in MST or not
        
        for (int e = 0; e < u->edges.size(); e++){
            Edge uvEdge = u->edges[e];
            Vertex* v = uvEdge.dest; //v = current destination vertex from an edge in u (u,v)
            if (!v->inMST && uvEdge.weight < v->dist){ //v is not in MST and weight of edge (u,v) < dist of v
                v->dist = uvEdge.weight; //update v's distance, add it back to the priority queue and assign parent as u (for edges)
                v->parent = u; 
                minq.push(v);
            }
        }
    }

    for (int i = 1; i < vList.size(); i++){
        MST.push_back(Edge(vList[i]->parent, vList[i], vList[i]->dist));
        //cout << '(' << vList[i]->parent->ID << ',' << vList[i]->ID << ')' << ":" << vList[i]->dist << " ";
    }
    return MST;
}

Vertex* cycStart;
Vertex* cycEnd;
bool DFS(Vertex* u, Vertex* p)
{
	if (!u->visit)
	{
		u->visit = true;
        u->inRecStack = true;

        for (int i = 0; i < u->edges.size(); i++){
            Vertex* v = u->edges[i].dest;
            if(!v->visit && DFS(v, u)){
                v->parent = u;
                return true;
            }
            else if (v->inRecStack && v != p){
                cycStart = u;
                cycEnd = v;
                return true;
            }        
        }

	}
    u->inRecStack = false; // vertex u no longer in recursion stack
	return false;
}

bool hasCycles(vector<Vertex*> vlist)
{
	// Call the recursive helper function to detect cycle in different
	// DFS trees
	for (int i = 0; i < vlist.size(); i++)
		if (DFS(vlist[i], NULL))
			return true;

	return false;
}

bool findvID(vector<Vertex*> vl, int vID){
    for (int i = 0; i < vl.size(); i++){
        if (vl[i]->ID == vID)
            return true;
    }
    return false;
}

void deleteEdgeMST(vector<Edge>& MST, Edge e){
    for (int i = 0; i < MST.size(); i++){
        if (MST[i].src->ID == e.src->ID && MST[i].dest->ID == e.dest->ID || MST[i].src->ID == e.dest->ID && MST[i].dest->ID == e.src->ID){
            MST.erase(MST.begin()+i);
        }
    }
}

int main(int argc, char** argv) 
{ 
    fstream inFile;
	string l;
    char t;
	int n,x;
	
	inFile.open(argv[1]);
	inFile >> t >> n; 
    getline(inFile, l);

	vector<int> rows;
    vector<int> cols;

	vector<Vertex*> Gv;
    for (int i = 0; i < n; i++){ // if n == 5, 0 .. 4 added
        Gv.push_back(new Vertex(i));
    }

    int srcid, destid, w;
    stringstream ss;

	for (int i = 0; i < n; i++){ //read n lines
		getline(inFile, l);
		ss.str(l); // Make an input stringstream to read from the line's string 
		ss >> srcid >> t; // where token == ':'
        if (ss.str().length()==3)
			continue;
		while (!ss.eof()){ // until end of the stream (line), keep reading in vertices (IDs)
			ss >> destid >> t >> w;
            Gv[srcid]->edges.push_back(Edge(Gv[srcid],Gv[destid],w));    
        }
		ss.clear(); //Clear for next iteration
	}
    getline(inFile, l);
    ss.str(l);  
    ss >> t >> srcid >> t >> destid >> t >> w; // u,v IDs for edge to delete 
    inFile.close();

    vector<Edge> MST1 = PrimsMST(Gv);

    string out = "MST1: ";
    int sumMST1 = 0;
    for (int i = 0; i < MST1.size(); i++){
        out += '(' + to_string(MST1[i].src->ID) + ',' + to_string(MST1[i].dest->ID) + ')';
        if (i != MST1.size()-1){
            //out += ' ';
        }
        sumMST1 += MST1[i].weight;
    }
    out += "\nSum MST1: " + to_string(sumMST1) + '\n';
    out += "Edge Added: ("+to_string(srcid) + ',' + to_string(destid) + ")\n";
     
    MST1.push_back(Edge(Gv[srcid], Gv[destid], w)); //Edge added to MST
    
    //COPYING current MST to a NEW vertex list (GRAPH), with current connected edges
    vector<Vertex*> MST1v; 
    for (int i = 0; i < n; i++){ 
        MST1v.push_back(new Vertex(i));
    }
    for (int i = 0; i < MST1.size(); i++){
        int s = MST1[i].src->ID, d = MST1[i].dest->ID, w = MST1[i].weight; 
        MST1v[s]->edges.push_back(Edge(MST1v[s], MST1v[d], w));
        MST1v[d]->edges.push_back(Edge(MST1v[d], MST1v[s], w)); //pushing both edges in both directions in corresponding vertices
    }
 
    //reset all visited to false
    for (int i = 0; i < MST1v.size(); i++){
        MST1v[i]->visit = false;
        MST1v[i]->inRecStack = false;
        MST1v[i]->parent = NULL;
    }

    hasCycles(MST1v);
    vector<Vertex*> cycleList;
    priority_queue<Edge, vector<Edge>, edgeWeightComparator> cycleEdges; 
    Vertex* currv = cycStart;

    while(currv != cycEnd){
        cycleList.push_back(currv);   
        currv = currv->parent;
    }
    cycleList.push_back(currv);

    for (int c = 1; c < cycleList.size(); c++){ //for every vertex in the cycle
        for (int i = 0; i < cycleList[c]->edges.size(); i++){ // in the vertices' adjacent edges
            if (cycleList[c-1]->edges[i].dest == cycleList[c]){
                cycleEdges.push(cycleList[c]->edges[i]);
            }
        }
    }

    for (int i = 0; i < cycStart->edges.size(); i++){ // in the vertices' adjacent edges
        if (cycStart->edges[i].dest == cycEnd){
            cycleEdges.push(cycStart->edges[i]); //edge found from start to end of cycle as well
            break;
        }
    }

    Edge CE = cycleEdges.top();
    // cout << CE.src->ID << " - " << CE.dest->ID  << " weight " << CE.weight << endl;
    deleteEdgeMST(MST1, CE);

    //4,1,0
    
    
    out += "MST2: ";
    sumMST1 = 0;
    for (int i = 0; i < MST1.size(); i++){
        out += '(' + to_string(MST1[i].src->ID) + ',' + to_string(MST1[i].dest->ID) + ')';
        if (i != MST1.size()-1){
            //out += ' ';
        }
        sumMST1 += MST1[i].weight;
    }
    out += "\nSum MST2: " + to_string(sumMST1);
    
    std::cout << out;

            
    return 0; 
} 

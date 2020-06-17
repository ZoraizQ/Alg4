#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <sstream>
using namespace std; 

// Time Complexity = O(ElogV), where E is the number of edges, while V is the number of vertices, logE for insertion in priority queue
// due to the Prims MST, which dominates all O(V+E) complexities for reconstruction the MST using BFS/DFS traversals. 
// to get the individual tree components after removal of the edge and finding the minimum replacement edge to connect them
// Data structure - vertex classes with edge vector for all adjacent edges, the graph is a vector of vertices, MST is a vector of edges. 
class Edge;

class Vertex
{ 
    public:
        int ID;
        int dist;
        bool visit;
        bool inMST;
        vector<Edge> edges;
        Vertex* parent;

	Vertex(int i){
		ID = i;
		visit = false;
        inMST = false;
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
        return e1.weight > e2.weight; 
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

void deleteEdgeMST(vector<Edge>& MST, Edge e){
    for (int i = 0; i < MST.size(); i++){
        if (MST[i].src == e.src && MST[i].dest == e.dest || MST[i].src->ID == e.dest->ID && MST[i].dest->ID == e.src->ID){
            MST.erase(MST.begin()+i);
        }
    }
}

vector<Vertex*> getReachable(Vertex* startV){
    vector<Vertex*> reachableVs;
    queue <Vertex*> BQ;
    BQ.push(startV); //initial vert added to BQueue

	while(BQ.size() != 0){
		Vertex* u = BQ.front(); //front popped
        u->visit = true;
        reachableVs.push_back(u);
		BQ.pop();
		for (int i = 0; i < u->edges.size(); i++){
		 	Vertex* v = u->edges[i].dest; // v == DEST
			if (v->visit == false){
				v->parent = u;
                BQ.push(v);
            }
		}
	}

    return reachableVs;
}

bool findvID(vector<Vertex*> vl, int vID){
    for (int i = 0; i < vl.size(); i++){
        if (vl[i]->ID == vID)
            return true;
    }
    return false;
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
    ss >> t >> srcid >> t >> destid >> t; // u,v IDs for edge to delete 
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
    out += "Edge Removed: ("+to_string(srcid) + ',' + to_string(destid) + ")\n";
    
    deleteEdgeMST(MST1, Edge(Gv[srcid], Gv[destid], -1));
    
    vector<Vertex*> MST1v; //copying current MST to a new vertex list, with current connected edges
    for (int i = 0; i < n; i++){ 
        MST1v.push_back(new Vertex(i));
    }
    for (int i = 0; i < MST1.size(); i++){
        int s = MST1[i].src->ID, d = MST1[i].dest->ID, w = MST1[i].weight; 
        MST1v[s]->edges.push_back(Edge(MST1v[s], MST1v[d], w));
        MST1v[d]->edges.push_back(Edge(MST1v[d], MST1v[s], w));
    }
 
    //reset all visited to false
    for (int i = 0; i < MST1v.size(); i++)
        MST1v[i]->visit = false;

    vector<Vertex*> Ta = getReachable(MST1v[srcid]); //u of the edge deleted becomes start of tree a

    if (Ta.size() != n){ //tree A does not have all the vertices 
        for (int i = 0; i < MST1v.size(); i++)
            MST1v[i]->visit = false;

        vector<Vertex*> Tb = getReachable(MST1v[destid]); //v of the edge deleted becomes start of tree b
        
        priority_queue<Edge, vector<Edge>, edgeWeightComparator> minEdgeQ;
        // need to get min possible edge that connects the two trees
        for (int i = 0; i < Ta.size(); i++){ //vertices in tree a
            vector<Edge> currEdgeL = Gv[Ta[i]->ID]->edges;
            for (int e = 0; e < currEdgeL.size(); e++){
                Edge currEdge = currEdgeL[e];
                if (!(currEdge.src->ID == srcid && currEdge.dest->ID == destid || currEdge.src->ID == destid && currEdge.dest->ID == srcid)){
                    minEdgeQ.push(currEdge);
                }
            }
        }
        
        int a, b, weightab; // edge src, dest IDs
        while (!minEdgeQ.empty()){ //e
            Edge top = minEdgeQ.top();
            //cout << top.src->ID << " " << top.dest->ID << endl;
            if (findvID(Tb, top.dest->ID)){//logv
                a = top.src->ID;
                b = top.dest->ID;
                break;
            }
            minEdgeQ.pop();
        }

        for (int i = 0; i < Gv[a]->edges.size(); i++){
            if (Gv[a]->edges[i].dest->ID == b){
                weightab = Gv[a]->edges[i].weight;
            }
        }
        MST1.push_back(Edge(Gv[a], Gv[b], weightab));
    }
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

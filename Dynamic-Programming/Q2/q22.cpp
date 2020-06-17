#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream> 
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std ;

struct edge ;

struct node
{
	int value ;
	int lighted ;
	int parent ; 

	vector <edge> edges ;

	node()
	{	
		value = -1 ;
		lighted = 0 ;
		parent = -1 ;
	}

	node(int v)
	{
		parent = -1 ;
		value = v ;
		lighted = 0 ;
	}
} ;

vector <node*> nodes ; // contains all the nodes from 0 to n along with edges

struct edge
{
	node *source ;
	node *dest ;

	edge()
	{
		source = NULL ;
		dest = NULL ;
	}

	edge(node *start, node *ending)
	{
		source = start ;
		dest = ending ;
	}
} ;

vector <node*> output ; // final ans

int lights(node *root, node *from) 
{
	if (root == NULL) // empty graph
		return 0 ;

	if (root -> parent == -1 && root -> edges.size() == 0) // if only one vertex
		return 0 ;

	// leaf nodes
	if (root -> edges.size() == 1 && from)
		return 0 ;

	// memoization proved helful here!
	if (root -> lighted > 0)
		return root -> lighted ;

	// Case 1 
	int including = 1 ;

	for (edge e : root->edges)
	{
		if(e.dest != from)
		{
			including += lights(e.dest, e.source) ;
		}
	}

	// Case 2
	int excluding = 0 ;

	for (edge e : root->edges)
	{
		if(e.dest != from)
		{
			excluding += 1 ;

			for (edge c : e.dest->edges)
			{
				if (e.source != c.dest)
				{
					excluding += lights(c.dest, c.source) ;
				}
			}
		}	
	}

	if (including > excluding)
		root -> lighted = excluding ;

	else
	{
		root -> lighted = including ;
		output.push_back(root) ;
	}

	return root -> lighted ;
} ;

int main(int argc, char** argv)
{
	char *f = argv[1] ;
    ifstream fin(f) ;
    string trash , v ;
    fin >> trash >> v ;
	int vertices = stoi(v) ;

	for (int i = 0; i < vertices ; i++)
	{
		node *temp = new node(i) ;
		nodes.push_back(temp) ;
	}

	string line ;
	
    bool check , flag = false ;
    int initial , final ;
    while (getline(fin , line))
    {
        int n = line.length() ;
        char arr[n+1] ;
        strcpy(arr , line.c_str()) ; 
        check = false ;
        char *ptr ;
        ptr = strtok(arr , " :") ;
 
        while (ptr != NULL)
        {
            if (check == false)
            {
                initial = stoi(ptr) ;
            }

            else
            {
                final = stoi(ptr) ;
            }

            check = true ;

            if (flag == false)
            {
                // do nothing
            }

            else
            {
				node *d = new node ;

				for (int i = 0; i < nodes.size(); i++) // finding destination node
				{
					if (nodes[i] -> value == final)
					{
						d = nodes[i] ;
						break ;
					}
				}

				edge e(nodes[initial], d) ; // edge between src and dest

				nodes[initial] -> edges.push_back(e) ;
            }

            ptr = strtok(NULL , " :") ;

            flag = true ;
        }

        flag = false ;
    }

    fin.close() ;

	node *root = nodes[0] ;
	
	int ans = lights(root, NULL) ;

	string out = "" ;

	out = out + "Minimum Lights " + to_string(ans) + "\n" + "{" ;

	for (int i = 0 ; i < output.size() ; i++)
	{
		int v = output[i] -> value ;

		if (i == output.size() - 1)
			out = out + to_string(v) + "}" + "\n" ;

		else
			out = out + to_string(v) + ", " ;
	}

	cout << out ;

	return 0 ;
}

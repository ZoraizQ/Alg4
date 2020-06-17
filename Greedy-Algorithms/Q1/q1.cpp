#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

using namespace std;

// structs for gamepiece and colData (order), time complexity = O(n^2logn),
// nlogn due to building and insertion of priority queue with n column data structs, and n rows

struct gamepiece{
	int r, c;
	gamepiece(int y, int x){
		r = y;
		c = x;
	}
};

struct colData{
	int colpcs, coli;
	colData(int pcs, int i){
		colpcs = pcs;
		coli = i;
	}
	bool operator<(const colData & cR) const // our own comparator
	{ 
		return (colpcs < cR.colpcs); 
	}
};

bool comparePieceR(gamepiece p1, gamepiece p2) // our own comparator
{ 
    return (p1.r <= p2.r && p1.c <= p2.c); 
}

bool pieceArrangements(string file, vector<gamepiece>& piecelist){
	fstream inFile;
	string t;
	int n,x;
	
	inFile.open(file.c_str());
	inFile >> t >> n; 

	vector<int> rows;
    vector<int> cols;
	priority_queue<colData> colsByMaxTotal; //first int is the pieces required and second is the row index

	inFile >> t;
	for (int i = 0; i < n; i++){
		inFile >> x;
		rows.push_back(x);
	}
	inFile >> t;
	for (int i = 0; i < n; i++){
		inFile >> x;
		cols.push_back(x);
		colsByMaxTotal.push(colData(x, i)); // insert row data to heap (index + total pieces), sorted by max total first
	}
	inFile.close();
	
	vector<colData> requeueCols;

	for (int r=0; r < n; r++){ //for all n rows, r = row index
		requeueCols.clear();
		for (int p = 0; p < rows[r]; p++){ //for number of pieces in row r
			colData currCol = colsByMaxTotal.top(); //get max column, remove from queue as well
			int colpcs = currCol.colpcs;
			int coli = currCol.coli;	
			colsByMaxTotal.pop();

			if (colpcs == 0){
				return false;
			}
			else{ //colpcs still not zero, needs more pieces
				piecelist.push_back(gamepiece(r, coli)); //add gamepiece found to piecelist
				colpcs--;	//decrement column pieces total since one was just added
				cols[coli]--; 
				requeueCols.push_back(colData(colpcs, coli)); // if the column pieces requirement is not satisfied add it toe the requeue
			}	
		}

		for(int i =0; i < requeueCols.size(); i++){
			colsByMaxTotal.push(requeueCols[i]);
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	bool possible = false;
	vector<gamepiece> piecelist;
	possible = pieceArrangements(argv[1], piecelist);
	
	string ans ="";
	if (!possible){
		ans = "Not Possible\n"; 
	}
	else{
		//sort(piecelist.begin(), piecelist.end(), comparePieceR);
		int noPieces = piecelist.size();
		for (int i = 0; i < noPieces; i++){
			if (i > 0 && piecelist[i].r > piecelist[i-1].r){ //add pieces to answer
				ans = ans.substr(0,ans.length()-1) + '\n';
			}
			ans += '('+to_string(piecelist[i].r+1) + ',' + to_string(piecelist[i].c+1)+')';
			if (i != noPieces-1){
				ans += ' ';
			}
		}
		ans += '\n';
	}
	std::cout<<ans;
	return 0;
}

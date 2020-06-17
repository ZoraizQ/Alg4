#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
using namespace std; 
  
vector<vector<pair<int, int>>> boomerangs;
int boomerangNum = 1;

int getQuadrant(int** board, int xmid, int ymid, int bx, int by){
    // cout << bx << " " << by << " " << xmid << " " << ymid << endl;
    int q = 0;
    if (bx < xmid && by < ymid){
        q = 1;
    }
    else if (bx < xmid && by >= ymid){
        q = 2;
    }
    else if (bx >= xmid && by < ymid){
        q = 3;
    }
    else{
        q = 4;
    }
    return q;
}

void placeBoomerang(int** board, int x1, int y1, int x2, int y2, int q){    
    boomerangs.push_back(vector<pair<int,int>>());
    int placer = 1;
    for (int i=x1;i<x2;i++){
        for (int j=y1;j<y2;j++){
            if (board[i][j] == 0 && placer != q){
                board[i][j] = boomerangNum;
                boomerangs[boomerangNum-1].push_back(make_pair(i, j));
            }
            placer++;
        }
    }
    boomerangNum++;
}

void boomerangFiller(int** board, int x1, int y1, int x2, int y2, int bx, int by){ //(x1,y1) top-left sub-board coordinate, (x2,y2) top-right sub-board coordinate
    int n = x2-x1; //length of board, use any since x2-x1 == y2-y1
    int xmid = (x1+x2)/2, ymid = (y1+y2)/2;
    if (n == 2){ //2x2 sub-board is the base case
        placeBoomerang(board, x1, y1, x2, y2, getQuadrant(board, xmid, ymid, bx, by));
    }
    else{ //greater than 2x2, place boomerang in middle with s.t sub-boards have each 1 blocked square total
        // cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl; cout << "Quad: " << getQuadrant(board, xmid, ymid, bx, by) << endl;    
        placeBoomerang(board, xmid-1, ymid-1, xmid+1, ymid+1, getQuadrant(board, xmid, ymid, bx, by));
        /* center is like
            (xmid-1,ymid-1) (xmid-1,ymid)
            (xmid, ymid-1)  (xmid, ymid)
        */
        int origbx = bx, origby = by;
        if (board[xmid-1][ymid-1] != 0){ //blocked TL
            bx = xmid-1;
            by = ymid-1;
        } //else original bx, by for unblocked
        boomerangFiller(board, x1, y1, xmid, ymid, bx, by); //TL 1
        
        if (board[xmid][ymid-1] != 0){ //blocked BL
            bx = xmid;
            by = ymid-1;
        }
        else{
            bx = origbx;
            by = origby;
        }
        boomerangFiller(board, xmid, y1, x2, ymid, bx, by); //BL 3
        
        if (board[xmid-1][ymid] != 0){ //blocked TR
            bx = xmid-1;
            by = ymid;
        }
        else{
            bx = origbx;
            by = origby;
        }
        boomerangFiller(board, x1, ymid, xmid, y2, bx, by); //TR 2
        
        if (board[xmid][ymid] != 0){ //blocked BR
            bx = xmid;
            by = ymid;
        }
        else{
            bx = origbx;
            by = origby;
        }
        boomerangFiller(board, xmid, ymid, x2, y2, bx, by); //BR 4
    }
}

int main(int argc, char** argv) 
{ 
    ifstream inFile;
	char t;
	int i, j, n, bx, by;
	string l, s;
	
	inFile.open(argv[1]);
	inFile >> t >> n;
	getline(inFile, l);
	vector<int> pair;

	inFile >> t >> bx >> t >> by >> t; //(x,y)
    inFile.close();
	// n x n grid
    // cout << x << "," << y << endl;
    int** board = new int*[n];
	for(i=0; i<n; i++){
		board[i] = new int[n];
	} //nxn 2d dynamic grid
    // all elements 0 initially except for the blank
    board[bx][by] = -1; //-1 for blank, 0 for empty space

    boomerangFiller(board, 0, 0, n, n, bx, by); //entire board passed initially
    // for(i=0;i<n;i++){
    //     for(j=0;j<n;j++){
    //         if (board[i][j] < 10 && board[i][j] >= 0){
    //             cout << ' ';
    //         }
    //         cout << board[i][j] << ' ';
    //     }
    //     cout << endl;
    // }

    string out = "";
    for (i=0;i<boomerangs.size();i++){
        out += to_string(i+1) + " ("+to_string(boomerangs[i][0].first)+","+to_string(boomerangs[i][0].second)+") ("+to_string(boomerangs[i][1].first)+","+to_string(boomerangs[i][1].second)+") ("+to_string(boomerangs[i][2].first)+","+to_string(boomerangs[i][2].second)+") " + "\n"; //trailing spaces and endlines dont matter
    }
    cout << out;
    return 0; 
} 

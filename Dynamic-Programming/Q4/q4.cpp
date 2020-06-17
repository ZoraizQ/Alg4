#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

int main(int argc, char** argv)
{
	ifstream inFile;
	char t;
	string S1, S2, S3;
	vector<pair<int, string>> outList;
	inFile.open(argv[1]);
	inFile >> S1 >> S2 >> S3;
	inFile.close();

	int m = S1.length(), n = S2.length(), sIDmatched;
	string charMatched;
	//bottom-up approach/tabulation by 2D grid m*n size, fixed
	bool grid[m+1][n+1]; //all are false initially
	for (int i = 0; i < m+1; i++){ //horizontal = S1
		for (int j = 0; j < n+1; j++){ //vertical = S2
			grid[i][j] = false;
		}
	}
	grid[0][0] = true; //since empty strings interleave an empty string
	char currS1, currS2, currS3;
	
	/*
	    0 1 2 3
	    - a a b (S1)
	0 - T T T F
	1 a T T F F
	2 x F T T T	
	3 y F F F T
	(S2)

	aaxaby (VALID)
	*/

	int pos1 = 0, pos2 = 0;
	for (int i = 0; i < m+1; i++){ //horizontal = S1
		for (int j = 0; j < n+1; j++){ //vertical = S2
	
			currS1 = S1[i-1], currS2 = S2[j-1], currS3 = S3[i+j-1];
			sIDmatched = 0;
			if (i == 0 && j == 0){
				continue;
			}
			if (i == 0){ //initializing first column, S1 is empty
				if (currS2 == currS3){ //if current char of S2 matches with S3, take cell's value from above (vert)
					grid[i][j] = grid[i][j-1];
				}	
			}
			else if (j == 0){ //initializing first row, S2 is empty
				if (currS1 == currS3){ 
					grid[i][j] = grid[i-1][j]; //take cell's value from left (hor)
				}
			}
			else if (currS1 == currS3 && currS2 != currS3){ //S1 matches only
				grid[i][j] = grid[i-1][j]; //get from left	
			}
			else if (currS2 == currS3 && currS1 != currS3){ //S2 matches only
				grid[i][j] = grid[i][j-1]; //get from above
			}
			else if (currS2 == currS3 && currS1 == currS3){ //both S1 and S2 chars match with S3
				grid[i][j] = grid[i][j-1] || grid[i-1][j]; //get from either above or left
			}			
		}
	}

	//optimal value with answer true/false will be stored in the last / bottom right most cell
	bool isValid = grid[m][n]; 
	
	
	string ans="";
	if (!isValid){
		ans+="INVALID";
	}
	else{
		ans +="VALID";
		int ci = m, cj = n; //current i,j, starting from optimal value
		int lastDir = 0;
		string conv;
		while (ci > 0 || cj > 0){ //base case
			bool left = false, up = false;

			if (ci > 0)		
				left = grid[ci-1][cj];
			
			if (cj > 0)
				up = grid[ci][cj-1];

			if (left){
				conv = S1[ci-1];
				if (lastDir == 1){
					outList[outList.size()-1].second = conv + outList[outList.size()-1].second;
				}
				else{ //empty list, new
					outList.push_back(make_pair(1, conv));	
				}
				ci--;
				lastDir = 1; //left
			}
			else if (up){
				conv = S2[cj-1];
				if (lastDir == 2){
					outList[outList.size()-1].second = conv + outList[outList.size()-1].second;
				}
				else{
					outList.push_back(make_pair(2, conv));	
				}
				cj--;
				lastDir = 2; //up
			}
		}

		for (int i = outList.size()-1; i >= 0; i--){
			ans += '\n' + to_string(outList[i].first) + ":" + outList[i].second;		
		}
	}
	cout<<ans;
	return 0;
}
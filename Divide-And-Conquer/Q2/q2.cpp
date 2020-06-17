#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct Measurement{
	int day, cells;
	Measurement(int d, int c){
		day = d;
		cells = c;
	}
};


int mergeAndCount(int begin, int middle, int end, vector<Measurement> &results, vector<vector<int>> &failedDayPairs){ 
    int nL = middle - begin + 1, nR = end - middle; //number of elements in left half, and right half 
    vector<Measurement> resultsLeft, resultsRight; //arrays for left and right halfs
    
	for (int l = 0; l < nL; l++)
		resultsLeft.push_back(results[begin+l]);

	for (int r = 0; r < nR; r++)
		resultsRight.push_back(results[r+middle+1]);
  
    int l = 0, r = 0, c = begin; //starting indices, left results half, right results half, original results array	
	while (l < nL && r < nR){
		Measurement currLeft = resultsLeft[l], currRight = resultsRight[r];
		if (currLeft.cells <= currRight.cells){
			results[c] = currLeft;
			l++;
		}
		else{
			results[c] = currRight;
			r++;
		}
		c++;
	}

    // add remaining elements of resultsLeft 
    while (l < nL){
		results[c] = resultsLeft[l];
		l++;
		c++;
	}
    // add remaining elements of resultsRight 
    while (r < nR){
		results[c] = resultsRight[r];
		r++;
		c++;
	}
	
	int failedtrials = 0;
	l = 0; r = 0;
	while (r < nR){
		if (resultsRight[r].cells < resultsLeft[l].cells/2.0){ // then it is going to be smaller than the halves of the next numbers as well
			// since they are sorted in ascending order
			for (int i = l; i < nL; i++)
				failedDayPairs[resultsRight[r].day].push_back(resultsLeft[i].day);
				
			failedtrials += (nL-l);
			r++;
			l = 0;
		}
		l++;
		if (l == nL){
			l = 0;
			r++;
		}
	}
	return failedtrials;
}

int countFailedTrials(int begin, int end, vector<Measurement> &results, vector<vector<int>> &failedDayPairs){ 
    if (begin < end){
        int middle = begin+(end-begin)/2;
        int ftLeft = countFailedTrials(begin, middle, results, failedDayPairs);
		int ftRight = countFailedTrials(middle+1, end, results, failedDayPairs); 
        int ftCurrent = mergeAndCount(begin, middle, end, results, failedDayPairs);
		return ftCurrent+ftLeft+ftRight;
	}
	else{
		return 0;
	}
}

		
int main(int argc, char** argv)
{
	ifstream inFile;
	char t;
	int x, i, j, n;
	string l, s;
	
	inFile.open(argv[1]);
	inFile >> t >> n;
	getline(inFile, l);
	
	vector<Measurement> results;
	vector<vector<int>> failedDayPairs(n); //indices represent day on right cell

	for (i = 0; i < n; i++){		
		inFile >> x;
		results.push_back(Measurement(i, x));
	}
	inFile.close();
	string out = "Failed Trials " + to_string(countFailedTrials(0, n-1, results, failedDayPairs)) + '\n';
	
	for (i = 0; i < failedDayPairs.size(); i++){
		int rowsize = failedDayPairs[i].size();
		for (j = 0; j < rowsize; j++){			
			out += '(' + to_string(i) + ',' + to_string(failedDayPairs[i][j]) + ')';
			if (j != rowsize-1)
				out += ' ';
		}
		if (rowsize != 0)
			out += '\n';
	}
	
	cout << out;
	return 0;
}
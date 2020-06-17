#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

char decode(int species1, int species2){
	if (species1 == species2){
		return 'Y';
	}
	else{
		return 'N';
	}
}

int findDominant(int begin, int end, vector<int> photos, int* dominantCount){
	if(end-begin == 1){ // base case
		return photos[begin];
	}

	int middle = (begin+end)/2, dom1Count = 0, dom2Count = 0;
	int dom1 = findDominant(begin, middle, photos, dominantCount); //O(logN) calls, array split into half every time 
	int dom2 = findDominant(middle, end, photos, dominantCount); //find dominant species in photos' subarray
	
	for(int i=begin; i<end; i++){ //O(N) comparison
		if(decode(dom1, photos[i]) == 'Y'){ //species in dominant species photo and current photo matches on decode
			dom1Count++;
		}
		if(decode(dom2, photos[i]) == 'Y'){
			dom2Count++;
		}
	}
	if(dom1Count>dom2Count){
		*dominantCount = dom1Count; //dominant 1 
		return dom1;
	}
	else{
		*dominantCount = dom2Count;
		return dom2;
	}
}

int main(int argc, char** argv)
{
	ifstream inFile;
	char t;
	int n, m, x, i, j;
	string l, s;
	
	inFile.open(argv[1]);
	inFile >> t >> n;
	getline(inFile, l);
	inFile >> t >> m;
	getline(inFile, l);
	
	vector<int> photos(n);
	for (i = 0; i < n; i++){		
		inFile >> photos[i];
	}
	// n encrypted photos, and  at most ‘​m’ different species, where ​m<n​. The ‘m’ species are identified using integer values from 0 to m-1
	
	int dominantCount = 0;
	int dominantSpecies = findDominant(0, n, photos, &dominantCount);
	string dominantSpeciesIndices = "";
	for (i = 0; i < n; i ++){
		if (decode(photos[i], dominantSpecies) == 'Y'){
			dominantSpeciesIndices += to_string(i) + ' ';
		}
	}

	string out = "Failure";
	if (dominantCount > n/2.0){ //determine if there are more than n/2 photos that belong to the same species or not.
		out = "Success";
		out += "\nDominant Species Count " + to_string(dominantCount);
		out += "\nDominant Species Indices " + dominantSpeciesIndices.substr(0, dominantSpeciesIndices.length()-1);
	}
	
	cout << out;

	return 0;
}

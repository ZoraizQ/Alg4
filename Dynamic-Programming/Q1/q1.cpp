#include <iostream>
#include <fstream>

using namespace std;

/* 
RECCURENCE RELATION: 
  base cases: maxRevenue(0) = max(RL[0], RH[0])
  maxRevenue(i) = max(RL[i] + maximize(i-1) , RH[i] +maximize(i-2))
  time complexity: O(n)
  subproblems: n, time to compute each subproblem: O(1)
*/

int *memoArr, *jobType;

int getMaxAndType(int low, int high, int i){
  if (low >= high){
    jobType[i] = 1;
    return low;
  }
  else{
    jobType[i] = 2;
    if (i >= 1){
      jobType[i-1] = 3;
      if(jobType[i-2] == 3){ //if a high job had been found previously and there had been a priming for it
        jobType[i-2] = 1; //turn that to low, since we're taking this higher job
      }
    }
    return high;
  }
}

int maxRevenue(int i, const int RH[], const int RL[]){
	if(memoArr[i] != -1){ //if the answer is in the memo return, recursive call not required
		return memoArr[i];
	}
	else{
		memoArr[i] = getMaxAndType(RL[i]+maxRevenue(i-1, RH, RL), RH[i]+maxRevenue(i-2, RH, RL), i);  //basically takes the max while doing additional stuff for output
		return memoArr[i];
	}
}

int main(int argc, char** argv){
  ifstream inFile;
	string t;
  int n, x, i;
	
	inFile.open(argv[1]);
	inFile >> t >> n;
	inFile >> t;
  int RH[n];
  int RL[n];
  memoArr = new int(n);
  jobType = new int(n);

  for (i = 0; i < n; i++){
    inFile >> RH[i];
  }
  inFile >> t;
  for (i = 0; i < n; i++){
    inFile >> RL[i];
  }
  inFile.close();
  for (i = 0; i < n; i++){
    memoArr[i] = -1; //no memory initally
  }
  memoArr[0] = getMaxAndType(RL[0], RH[0], 0);

  int totalRev = maxRevenue(n-1, RH, RL);
  string ans = "";
  for (i = 0; i < n; i++){
    ans += "Week " + to_string(i+1) + ": ";
    switch(jobType[i]){
      case 1:
        ans += "Low " + to_string(RL[i]);
        break;
      case 2:
        ans += "High " + to_string(RH[i]);
        break;
      case 3:
        ans += "Priming";
        break;
    }
    ans += '\n';
  }
  ans += "Total Revenue: " + to_string(totalRev);
  std::cout << ans << endl;
  
}

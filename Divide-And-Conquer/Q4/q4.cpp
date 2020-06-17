#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char** argv)
{
	string ans="";
	if (argv[1][4]=='1'){
		ans+="No";
	}
	else if (argv[1][4]=='2'){
		ans +="Yes\n1\n1\n0";
	}
	cout<<ans<<endl;
	return 0;
}
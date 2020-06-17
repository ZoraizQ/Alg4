#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <sstream>
using namespace std; 

//Time complexity = 2^n, since this is a brute force algorithm that tries all doors one by one (in order) until it is possible to keep all the rooms open 

class Room;

class Door{
	public:
		int label;
		bool anti, open;
		
		Door(){
			label = -1;
			anti = false;
			open = false;
		}
		Door(int l, bool a){
			label = l;
			anti = a;
			open = false;
		}	
};

vector<Room*> RoomsList;

class Room{
	public:
		bool roomOpen;
		Door* d1, * d2;

		Room(Door* one, Door* two){
			d1 = one;
			d2 = two;
			roomOpen = false;
		}

		void updateDoor(int num, Door* d){
			//cout << "updating door" << endl;
			if (num == 0){
				//cout << "updating Door " << d1->label << "-" << d1->anti << "-" << d1->open << " with " << d->label << "-" << d->anti << "-" << d->open << endl;
				if (d1->anti != d->anti){
					d1->open = false;
				}
				else{
					d1->open = d->open;
				}
			}
			else if (num == 1){ // second door open
				//cout << "updating Door " << d2->label << "-" << d2->anti << "-" << d2->open << " with " << d->label << "-" << d->anti << "-" << d->open << endl;
				if (d2->anti != d->anti){					
					d2->open = false;
				}
				else{
					d2->open = d->open;
				}
			}
		}

		void simulateDoorOpen(int num){
			Door* currDoor;
			if (num == 0){
				d1->open = true;
				currDoor = d1;
			}
			else{
				d2->open = true;
				currDoor = d2;
			}

			for (int r = 0; r < RoomsList.size(); r++){
				Room* currRoom = RoomsList[r];
				if (currRoom->d1->label == currDoor->label){
					currRoom->updateDoor(0, currDoor);
				}
				
				if (currRoom->d2->label == currDoor->label){	
					currRoom->updateDoor(1, currDoor);
				}
			}
		}

		bool updateOpen(){
			if (d1->open || d2->open){
				roomOpen = true;
			}
			else{
				roomOpen = false;
			}
			return roomOpen;
		}
};

void printRoomsList(){
	for (int r = 0; r < RoomsList.size(); r++){
		cout << "d";
		Room* currRoom = RoomsList[r];
		if (currRoom->d1->anti){
			cout << "a";	
		}
		cout << currRoom->d1->label;
		if (currRoom->d1->open){
			cout << " open";	
		}
		else{
			cout << " close";
		}
		cout << "  ";
		cout << "d";
		if (currRoom->d2->anti){
			cout << "a";	
		}
		cout << currRoom->d2->label;
		if (currRoom->d2->open){
			cout << " open";	
		}
		else{
			cout << " close";
		}
		cout << endl;
	}
}

bool allRoomsOpen(){
	bool allOpen = true;
	for (int r = 0; r < RoomsList.size(); r++){
		if (!RoomsList[r]->updateOpen()){
			allOpen = false;
		}
	}
	return allOpen;
}

bool tryCombos(){
	//try forward and reverse combos
	for (int r = 0; r < RoomsList.size(); r++){
		for (int d = 0; d < 2; d++){
			RoomsList[r]->simulateDoorOpen(d);
			if (allRoomsOpen()){
				return true;
			}
		}
	}
	for (int r = RoomsList.size()-1; r >= 0; r--){
		for (int d = 0; d < 2; d++){
			RoomsList[r]->simulateDoorOpen(d);
			if (allRoomsOpen()){
				return true;
			}
		}
	}
	return false;
}

int main(int argc, char** argv)
{
	fstream inFile;
	string l,DoorStr;
    char t;
	int n, k, x; //Rooms
	vector<bool> openClose;

	inFile.open(argv[1]);
	inFile >> t >> n; 
	inFile >> t >> k; 
    getline(inFile, l);

	for (int i = 0; i < k; i++){
		openClose.push_back(false);
	}

    stringstream ss;
	for (int i = 0; i < n; i++){ //read n lines
		getline(inFile, l);
		ss.str(l); // Make an input stringstream to read from the line's string 
		int DoorID1, DoorID2;
		bool a1, a2;
		while (!ss.eof()){
		ss >> DoorStr;
		//cout << DoorStr << endl;
		DoorID1 = (int)DoorStr[DoorStr.length()-1] - 48; // char to int
		if (DoorStr.length() == 2){ //e.g. d0
			a1 = false;
		}
		else{ //e.g. da0
			a1 = true;
		}
		ss >> DoorStr;
		DoorID2 = (int)DoorStr[DoorStr.length()-1] - 48; // char to int
		if (DoorStr.length() == 2){ //e.g. d0
			a2 = false;
		}
		else{ //e.g. da0
			a2 = true;
		}	
		}
		RoomsList.push_back(new Room(new Door(DoorID1,a1), new Door(DoorID2,a2)));
		ss.clear(); //Clear for next iteration
	}
	inFile.close();
	
	bool possible = tryCombos();
	// printRoomsList();
	// std::cout << "----" << endl;
	// RoomsList[1]->simulateDoorOpen(1);
	// RoomsList[0]->simulateDoorOpen(1);
	// printRoomsList();

	//cout << tryCombos() << endl;
	
	// printRoomsList();

	string out = "Yes\n";
	if (!possible){
		out = "No\n";
	}
	else{
		for (int r = 0; r < RoomsList.size(); r++){
			Room* currRoom = RoomsList[r];
			int i = currRoom->d1->label;
			if (currRoom->d1->anti){
				openClose[i] = !currRoom->d1->open;
			}
			else{
				openClose[i] = currRoom->d1->open;
			}

			i = currRoom->d2->label;
			if (currRoom->d2->anti){
				openClose[i] = !currRoom->d2->open;
			}
			else{
				openClose[i] = currRoom->d2->open;
			}
			
		}

		for (int i = 0; i < openClose.size(); i++){
			if (openClose[i]){
				out += "1";
			}
			else{
				out += "0";
			}
			out += "\n";
		}
	}
	cout << out;
	
	return 0;
}
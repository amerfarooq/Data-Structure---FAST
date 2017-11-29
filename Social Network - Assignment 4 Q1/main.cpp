#include "AdjacencyList.h"
#include "SocialNetwork.h"

using namespace std;
		
void displayMenu() {
	cout << endl;	
	cout << "0- Exit" << endl;
	cout << "1- Find person with the most friends" << endl;
	cout << "2- Find person with the least friends" << endl;
	cout << "3- Find person with a specific amount of friends" << endl;
	cout << "4- Find the mutual friends of two profiles" << endl;
	cout << "5- Perform a BFS" << endl;
	cout << "6- Perform a DFS" << endl << endl;
}


int main() {

	ifstream inFile("C:\\Users\\New\\Desktop\\DS Assignment 4\\friends.csv");
	if (!inFile) {
		cerr << "\nFile not found! Please edit the directory path in main.cpp";
		return 0;
	}
	
	SocialNetwork network(inFile);

	while (true) {
		displayMenu();
		
		int option;
		cout << "\nEnter option: ";
		cin >> option;

		if (option < 0 || option > 6) {
			cout << "\nInvalid Option!" << endl;
			continue;
		}
		switch (option) {
			case 0:
				return 0;
			case 1:
				network.getPersonsWithMaxFriends();
				break;
			case 2:
				network.getPersonsWithMinFriends();
				break;
			case 3:
				network.getPersonsWithSpecificFriends();
				break;
			case 4:
				network.getMutualFriends();
				break;
			case 5:
				network.BFS();
				break;
			case 6:
				network.DFS();
				break;
		}
	}
	

	system("Pause");
}
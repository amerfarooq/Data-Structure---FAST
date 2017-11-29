#include "AdjacencyList.h"

using namespace std;
		
int main() {

	ifstream inFile("C:\\Users\\New\\Desktop\\DS Assignment 4\\friends.csv");
	if (!inFile) {
		cerr << "Error reading file!";
		return 0;
	}
	
	AdjacencyList ajList(inFile);

	auto mFriends = ajList.findMutualFriends("Hassan", "Sami");
	if (mFriends.second) {
		for (auto p : mFriends.first)
			cout << p << endl;
	}
	
	system("Pause");
}
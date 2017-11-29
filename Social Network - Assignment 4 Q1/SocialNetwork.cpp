#include "SocialNetwork.h"

SocialNetwork::SocialNetwork(ifstream& file) {
	ajList = AdjacencyList::getAdjacencyList(file);
}

void SocialNetwork::getPersonsWithMaxFriends() const {
	auto persons = ajList->getPersonMostFriends();
	cout << "\nProfiles with the most friends: " << endl;
	for (const auto p : persons)
		cout << "  " << p << endl;
}

void SocialNetwork::getPersonsWithMinFriends() const {
	auto persons = ajList->getPersonLeastFriends();
	cout << "\nProfiles with the least friends: " << endl;
	for (const auto p : persons)
		cout << "   " << p << endl;
}

void SocialNetwork::getPersonsWithSpecificFriends() const {
	size_t friendCount;
	cout << "\nEnter friend count: ";
	cin >> friendCount;

	if (!cin || friendCount <= 0) { cerr << "\nInvalid Number!" << endl; return; }

	auto persons = ajList->getPersonSpecificFriends(friendCount);
	if (persons.empty()) {
		cout << "\nNo such profile exists!" << endl;
		return;
	}
	cout << "\nProfiles with " << friendCount << " amount of friends: " << endl;
	for (const auto p : persons)
		cout << "   " << p << endl;
}

void SocialNetwork::getMutualFriends() const {
	string personA;
	cout << "\nEnter first profile name: ";
	cin >> personA;

	string personB;
	cout << "Enter second profile name: ";
	cin >> personB;

	auto persons = ajList->findMutualFriends(personA, personB);
	if (!persons.second) cout << "\nNo mutual friends exist!" << endl;
	else {
		cout << "\nMutual friends of " << personA << " and " << personB << ": " << endl;
		for (auto& p : persons.first)
			cout << "   " << p << endl;
	}
}

void SocialNetwork::printAllProfiles() const {
	ajList->print();
}

void SocialNetwork::BFS() const {
	ofstream outFile("BFS.txt");
	
	string start;
	cout << "\nInitiate search from node: ";
	cin >> start;

	ajList->BFS(start, outFile);
	ajList->reset();
}

void SocialNetwork::DFS() const {
	ofstream outFile("DFS.txt");

	string start;
	cout << "\nInitiate search from node: ";
	cin >> start;

	ajList->DFS(start, outFile);
	ajList->reset();
}



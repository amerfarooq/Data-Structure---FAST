#pragma once

#include "SocialNetwork.h"
#include "AdjacencyList.h"

using namespace std;


class SocialNetwork {

public:
	SocialNetwork(ifstream& file);
	
	void getPersonsWithMaxFriends() const;
	void getPersonsWithMinFriends() const;
	void getPersonsWithSpecificFriends() const;
	void getMutualFriends() const;
	void printAllProfiles() const;
	void BFS() const;
	void DFS() const;


private:
	unique_ptr<AdjacencyList> ajList;
};


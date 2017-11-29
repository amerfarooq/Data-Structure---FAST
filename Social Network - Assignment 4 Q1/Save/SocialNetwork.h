#pragma once

#include "SocialNetwork.h"
#include "AdjacencyList.h"

using namespace std;


class SocialNetwork {

public:
	SocialNetwork(ifstream& file);



private:
	unique_ptr<AdjacencyList> ajList;
};


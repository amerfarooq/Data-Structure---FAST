#include "SocialNetwork.h"

SocialNetwork::SocialNetwork(ifstream& file) {
	ajList = AdjacencyList::getAdjacencyList(file);
}



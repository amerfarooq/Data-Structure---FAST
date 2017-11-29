#include "AdjacencyList.h"



// Public functions


/*
 *	CTOR
 */
AdjacencyList::AdjacencyList(ifstream& file, size_t size /* = 30 */) {
	this->profiles = new pair<Node, bool>[size];
	this->size = size;
	construct(file);
}
/*
 *	Creats an adjacency list and returns it as a unique_ptr
 */
unique_ptr<AdjacencyList> AdjacencyList::getAdjacencyList(ifstream & file) {
	return make_unique<AdjacencyList>(file);
}
/*
 *	Returns mutual friends of 2 persons. Pair's second will be true if mutual friends
 *  exist and false if they dont.
 */
pair<vector<string>, bool> AdjacencyList::findMutualFriends(const string& a, const string& b) {

	if (a == b || !areFriends(a, b))
		return (make_pair(vector<string>(), false));

	auto friendLists = getFriendLists(a, b);
	vector<string> mFriends;

	if (friendLists.first->getFriends() <= friendLists.second->getFriends())
		mFriends = extractMutualFriends(friendLists.first, friendLists.second);
	else
		mFriends = extractMutualFriends(friendLists.second, friendLists.first);

	return mFriends.size() == 0 ? make_pair(vector<string>(), false) :
								  make_pair(mFriends, true);
}
/*
 *	Returns the most friends a single person has.
 */
size_t AdjacencyList::getMaxFriends() const {
	auto maxElem = max_element(profiles, profiles + currIndex,
		[](const pair<Node, bool>& lhs,
		   const pair<Node, bool>& rhs) {
		return lhs.first.getFriends() < rhs.first.getFriends(); });

	return (*maxElem).first.getFriends();
}
/*
 *	Returns the least friends a single person has.
 */
size_t AdjacencyList::getLeastFriends() const {
	auto minElem = min_element(profiles, profiles + currIndex,
		[](const pair<Node, bool>& lhs,
		   const pair<Node, bool>& rhs) {
		return lhs.first.getFriends() < rhs.first.getFriends(); });

	return (*minElem).first.getFriends();
}
/*
 *	Returns a vector containing names of all the people with lowest amount of friends.
 */
vector<string> AdjacencyList::getPersonLeastFriends() const {
	vector<string> persons;
	size_t leastFriends = getLeastFriends();

	for_each(profiles, profiles + currIndex,
		[&](const pair<Node, bool>& elem) {
		if (elem.first.getFriends() == leastFriends)
			persons.push_back(elem.first.name);
		});
	return persons;
}
/*
 *	Returns a vector containing names of all the people with most amount of friends.
 */
vector<string> AdjacencyList::getPersonMostFriends() const {
	vector<string> persons;
	size_t mostFriends = getMaxFriends();

	for_each(profiles, profiles + currIndex,
		[&](const pair<Node, bool>& elem) {
		if (elem.first.getFriends() == mostFriends)
			persons.push_back(elem.first.name);
		});
	return persons;
}
/*
 *	Returns a vector containing names of all the people with a specific amount of friends.
 */
vector<string> AdjacencyList::getPersonSpecificFriends(size_t num) const {
	vector<string> persons;

	for_each(profiles, profiles + currIndex,
		[&](const pair<Node, bool>& elem) {
		if (elem.first.getFriends() == num)
			persons.push_back(elem.first.name);
	});
	return persons;
}
/*
 *	Performs a bread-first search on the graph.
 */
void AdjacencyList::BFS(const string& name, ofstream& txtFile) const {
	auto findNode = getPair(name);

	if (findNode->second) {	// In case name is not found in profiles
		cerr << "\nName not found!" << endl;
		return;
	}
	txtFile << findNode->first.name << endl;
	findNode->second = true;

	queue<pair<Node, bool>*> toVisit;
	toVisit.push(findNode);

	while (!toVisit.empty()) {
		auto queueFront = toVisit.front();
		toVisit.pop();

		Node* iterator = queueFront->first.next;
		while (iterator) {
			auto iterPair = getPair(iterator->name);

			if (!iterPair->second) {	// If Node is unvisited
				txtFile << iterPair->first.name << endl;
				iterPair->second = true;
				toVisit.push(iterPair);
			}
			iterator = iterator->next;
		}
	}
	// Check if a disjoint node is present and call BFS on it if it exists.
	auto disjointSubTree = find_if_not(profiles, profiles + currIndex,
			    [](const pair<Node, bool>& elem) {
			    return elem.second; });
	
	if (disjointSubTree != profiles + currIndex)
		BFS(disjointSubTree->first.name, txtFile);
}
/*
 *	Performs a depth-first search on the graph.
 */
void AdjacencyList::DFS(const string& name, ofstream& txtFile) const {
	auto findNode = getPair(name);

	if (findNode->second) {	// In case name is not found in profiles
		cerr << "\nName not found!" << endl;
		return;
	}
	stack<pair<Node, bool>*> toVisit;
	toVisit.push(findNode);

	while (!toVisit.empty()) {
		auto stackTop = toVisit.top();
	
		if (!stackTop->second) {
			txtFile << stackTop->first.name << endl;
			stackTop->second = true;
		}

		auto unvisitedFriend = getUnvisitedFriendPair(stackTop);
		if (!unvisitedFriend->second) {
			toVisit.push(unvisitedFriend);
			continue;
		}
		else 
			toVisit.pop();
	}
	// Check if a disjoint node is present and call BFS on it if it exists.
	auto disjointSubTree = find_if_not(profiles, profiles + currIndex,
		[](const pair<Node, bool>& elem) {
		return elem.second; });

	if (disjointSubTree != profiles + currIndex)
		DFS(disjointSubTree->first.name, txtFile);
}
void AdjacencyList::reset() {
	for (int i = 0; i < currIndex; ++i)
		profiles[i].second = false;
}
/*
 *	Prints name and friends of all profiles.
 */
void AdjacencyList::print() const {
	for (int i = 0; i < currIndex; ++i) {
		cout << profiles[i].first.name << ": ";
		profiles[i].first.print();
		cout << endl;
	}
}
/*
 *	DTOR
 */
AdjacencyList::~AdjacencyList() {
	delete[] profiles;
}



// Private functions


/*
 *	Constructs the graph using a csv file.
 */
void AdjacencyList::construct(ifstream& file) {
	while (file) {
		auto names = readPairOfNames(file);
		if (names.first == "" && names.second == "") continue;
		insertPairOfNames(names);
	}
}
/*
 *	Reads a single line from csv and returns both names as a pair.
 */
const pair<string, string> AdjacencyList::readPairOfNames(ifstream& file) {
	string line;

	getline(file, line);

	stringstream lineStream(line);
	string firstName,
		   secondName,
		   cell;
	bool isFirstName = true;

	while (getline(lineStream, cell, ',')) {
		if (isFirstName) {
			firstName = cell;
			isFirstName = false;
		}
		else secondName = cell;
	}
	return make_pair(firstName, secondName);
}
/*
 *	Inserts a pair of names into the adjancencyList.
 */
void AdjacencyList::insertPairOfNames(const pair<string, string>& names) {
	bool isFirstNamePresent = false,
		 isSecondNamePresent = false;

	// First check if Nodes of both names are available in profiles array 
	// and if any one match is found, the other name is added to that Node's list.
	for (int i = 0; i < this->currIndex; ++i) {

		if (!isFirstNamePresent && profiles[i].first.name == names.first) {
			profiles[i].first.addToList(names.second);
			profiles[i].second = false;
			isFirstNamePresent = true;
		}
		if (!isSecondNamePresent && profiles[i].first.name == names.second) {
			profiles[i].first.addToList(names.first);
			profiles[i].second = false;
			isSecondNamePresent = true;
		}
		if (isFirstNamePresent && isSecondNamePresent) break;
	}
	// In case the Node of a name is not found, make its Node at the currIndex of
	// profiles array.
	if (!isFirstNamePresent) {
		profiles[currIndex].first.name = names.first;
		profiles[currIndex].second = false;
		profiles[currIndex].first.addToList(names.second);
		++currIndex;
		if (currIndex == size) resize();
	}
	if (!isSecondNamePresent) {
		profiles[currIndex].first.name = names.second;
		profiles[currIndex].second = false;
		profiles[currIndex].first.addToList(names.first);
		++currIndex;
		if (currIndex == size) resize();
	}
}
/*
 *	Returns true if two persons are friends and false otherwise.
 */
bool AdjacencyList::areFriends(const string& a, const string& b) const {
	assert(a != b);

	for (int i = 0; i < currIndex; ++i) {
		const Node& profile = profiles[i].first;

		if (profile.name == a) return profile.search(b);
		if (profile.name == b) return profile.search(a);
	}
}
/*
 *	Returns the friend lists i.e. head Node's of both persons as a pair. 
 */
pair<const AdjacencyList::Node*, const AdjacencyList::Node*> AdjacencyList::getFriendLists(const string& a, const string& b) const {
	Node* listA;
	Node* listB;

	bool foundA = false;
	bool foundB = false;
	
	for (int i = 0; i < currIndex; ++i) {
		if (!foundA && profiles[i].first.name == a) {
			listA = &profiles[i].first;
			foundA = true;
		}
		if (!foundB && profiles[i].first.name == b) {
			listB = &profiles[i].first;
			foundB = true;
		}
		if (foundA && foundB) return make_pair(listA, listB);
	}
}
/*
 *	Returns a vector containing the mutual friends contained in both Nodes.
 */
vector<string> AdjacencyList::extractMutualFriends(const Node* shorter, const Node* larger) const {
	Node* iterS = shorter->next;
	Node* iterL = larger->next;
	vector<string> mFriends;

	while (iterS) {
		if (larger->search(iterS->name)) 
			mFriends.push_back(iterS->name);
		
		iterS = iterS->next;
	}
	return mFriends;
}
/*
 *	Returns the pair that matches name from the profiles array.
 */
pair<AdjacencyList::Node, bool>* AdjacencyList::getPair(const string& name) const {
	for (int i = 0; i < this->currIndex; ++i) {
		if (profiles[i].first.name == name)
			return &profiles[i];
	}
	return &make_pair(Node(), true);
}
/*
 *	Returns first unvisited pair from friend list of given name.
 */
pair<AdjacencyList::Node, bool>* AdjacencyList::getUnvisitedFriendPair(const pair<Node, bool>* name) const {
	Node* iterator = name->first.next;
	
	while (iterator) {
		auto searchPair = getPair(iterator->name);

		if (!searchPair->second)
			return searchPair;

		iterator = iterator->next;
	}
	return &make_pair(Node(), true);
}
/*
 *	Doubles array size when currIndex becomes equal to old size.
 */
void AdjacencyList::resize() {
	assert(currIndex == size);
	
	size *= 2;
	pair<Node, bool>* newProfiles = new pair<Node, bool>[size];

	for (int i = 0; i < currIndex; ++i) {
		newProfiles[i] = profiles[i];
	}
	auto temp = profiles;
	profiles = newProfiles;
	delete[] temp;
}




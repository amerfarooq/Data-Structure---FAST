#include "AdjacencyList.h"
#include <stack>


// Public functions

AdjacencyList::AdjacencyList(ifstream& file, size_t size /* = 30 */) {
	this->profiles = new HeadNode[size];
	this->size = size;
	construct(file);
}

unique_ptr<AdjacencyList> AdjacencyList::getAdjacencyList(ifstream & file) {
	return make_unique<AdjacencyList>(file);
}

pair<vector<string>, bool> AdjacencyList::findMutualFriends(const string& a, const string& b) {

	if (a == b || !areFriends(a, b))
		return (make_pair(vector<string>(), false));

	auto friendLists = getFriendLists(a, b);
	vector<string> mFriends;

	if (friendLists.first.totalFriends <= friendLists.second.totalFriends)
		mFriends = extractMutualFriends(friendLists.first, friendLists.second);
	else
		mFriends = extractMutualFriends(friendLists.second, friendLists.first);

	return mFriends.size() == 0 ? make_pair(vector<string>(), false) :
								  make_pair(mFriends, true);
}

size_t AdjacencyList::getMaxFriends() const {
	auto maxElem = max_element(profiles, profiles + currIndex, 
		[](const HeadNode& lhs, const HeadNode& rhs) { 
		return lhs.totalFriends < rhs.totalFriends; });
	
	return maxElem->totalFriends;
}

size_t AdjacencyList::getLeastFriends() const {
	auto minElem = min_element(profiles, profiles + currIndex,
		[](const HeadNode& lhs, const HeadNode& rhs) {
		return lhs.totalFriends < rhs.totalFriends; });

	return minElem->totalFriends;
}

vector<string> AdjacencyList::getPersonLeastFriends() const {
	vector<string> persons;
	size_t leastFriends = getLeastFriends();

	for_each(profiles, profiles + currIndex,
		[&](const HeadNode& elem) {
		if (elem.totalFriends == leastFriends)
			persons.push_back(elem.name);
		});
	return persons;
}

vector<string> AdjacencyList::getPersonMostFriends() const {
	vector<string> persons;
	size_t mostFriends = getMaxFriends();

	for_each(profiles, profiles + currIndex,
		[&](const HeadNode& elem) {
		if (elem.totalFriends == mostFriends)
			persons.push_back(elem.name);
	});
	return persons;
}

vector<string> AdjacencyList::getPersonSpecificFriends(size_t num) const {
	vector<string> persons;

	for_each(profiles, profiles + currIndex,
		[&](const HeadNode& elem) {
		if (elem.totalFriends == num)
			persons.push_back(elem.name);
	});
	return persons;
}

void AdjacencyList::DFS() const {

}

void AdjacencyList::print() const {
	for (int i = 0; i < currIndex; ++i) {
		cout << profiles[i].name << ": ";
		profiles[i].print();
		cout << endl;
	}
}

AdjacencyList::~AdjacencyList() {
	delete[] profiles;
}



// Private functions

void AdjacencyList::construct(ifstream& file) {
	while (file) {
		auto names = readPairOfNames(file);
		if (names.first == "" && names.second == "") continue;
		insertPairOfNames(names);
	}
}

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

void AdjacencyList::insertPairOfNames(const pair<string, string>& names) {
	bool isFirstNamePresent = false,
		isSecondNamePresent = false;

	// First check if HeadNodes of both names are available in profiles array 
	// and if any one match is found, the other name is added to that HeadNode's list.
	for (int i = 0; i < this->currIndex; ++i) {

		if (!isFirstNamePresent && profiles[i].name == names.first) {
			profiles[i].addToList(names.second);
			isFirstNamePresent = true;
		}
		if (!isSecondNamePresent && profiles[i].name == names.second) {
			profiles[i].addToList(names.first);
			isSecondNamePresent = true;
		}
		if (isFirstNamePresent && isSecondNamePresent) break;
	}
	// In case the HeadNode of a name is not found, make its HeadNode at the currIndex of
	// profiles array.
	if (!isFirstNamePresent) {
		profiles[currIndex].name = names.first;
		profiles[currIndex].addToList(names.second);
		++currIndex;
		if (currIndex == size) resize();
	}
	if (!isSecondNamePresent) {
		profiles[currIndex].name = names.second;
		profiles[currIndex].addToList(names.first);
		++currIndex;
		if (currIndex == size) resize();
	}
}

bool AdjacencyList::areFriends(const string& a, const string& b) const {
	assert(a != b);

	for (int i = 0; i < currIndex; ++i) {
		const HeadNode& profile = profiles[i];

		if (profile.name == a) return profile.search(b);
		if (profile.name == b) return profile.search(a);
	}
}

pair<const AdjacencyList::HeadNode, const AdjacencyList::HeadNode> AdjacencyList::getFriendLists(const string& a, const string& b) const {
	HeadNode listA, listB;

	bool foundA = false;
	bool foundB = false;
	
	for (int i = 0; i < currIndex; ++i) {
		if (!foundA && profiles[i].name == a) { 
			listA = profiles[i];
			foundA = true;
		}
		if (!foundB && profiles[i].name == b) {
			listB = profiles[i];
			foundB = true;
		}
		if (foundA && foundB) return make_pair(listA, listB);
	}
}

vector<string> AdjacencyList::extractMutualFriends(const HeadNode& shorter, const HeadNode& larger) const {
	ListNode* iterS = shorter.HeadNode;
	ListNode* iterL = larger.HeadNode;
	vector<string> mFriends;

	while (iterS) {
		if (larger.search(iterS->name)) 
			mFriends.push_back(iterS->name);
		
		iterS = iterS->next;
	}
	return mFriends;
}

void AdjacencyList::resize() {
	assert(currIndex == size);
	size *= 2;
	HeadNode* newProfiles = new HeadNode[size];

	for (int i = 0; i < currIndex; ++i) {
		newProfiles[i] = profiles[i];
	}
	HeadNode* temp = profiles;
	profiles = newProfiles;
	delete[] temp;
}




#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include <assert.h> 
#include <vector>

using namespace std;


class AdjacencyList
{
public:
	AdjacencyList(ifstream& file, size_t size = 30);
	static unique_ptr<AdjacencyList> getAdjacencyList(ifstream& file);
	
	void print() const;
	
	size_t getMaxFriends() const;
	size_t getLeastFriends() const;
	
	vector<string> getPersonLeastFriends() const;
	vector<string> getPersonMostFriends() const;
	vector<string> getPersonSpecificFriends(size_t n) const;

	void DFS() const;

	pair<vector<string>, bool> findMutualFriends(const string&, const string&);

	~AdjacencyList();

private:

	struct Node {
		Node* next;
		string name;
		bool isVisited = false;

		Node(const string& name = "") : next(nullptr), name(name) {}
		void resetVisitedStatus() { isVisited = false; }
		void visit() { isVisited = true; }
		~Node() { delete next; }
	};

	struct HeadNode : Node {
		size_t totalFriends = 0;

		HeadNode() = default;
		HeadNode(string name) : Node(name) {}

		void addToList(const string& name) {
			if (!next)
				next = new Node(name);
			else {
				Node* iterator = next;
				Node* prev = nullptr;

				while (iterator) {
					prev = iterator;
					iterator = iterator->next;
				}
				prev->next = new Node(name);
			}
			++totalFriends;
		}

		bool search(const string& name) const {
			Node* iterator = next;

			while (iterator) {
				if (iterator->name == name) return true;
				iterator = iterator->next;
			}
			return false;
		}

		//void resetForTraversal() {
		//	isVisited = false;
		//	Node* iter = HeadNode;
		//	while (iter) {
		//		iter->resetVisitedStatus();
		//		iter = iter->next;
		//	}
		//}

		void print() const {
			Node* iterator = next;
			while (iterator) {
				cout << iterator->name << ",";
				iterator = iterator->next;
			}
		}
	};

	HeadNode* profiles;
	size_t size;
	size_t currIndex = 0;

	void construct(ifstream& file);
	void insertPairOfNames(const pair<string, string>& names);
	const pair<string, string> readPairOfNames(ifstream& singeLine);
	void resize();
	bool areFriends(const string&, const string&) const ;
	pair<const HeadNode, const HeadNode> getFriendLists(const string&, const string&) const;
	vector<string> extractMutualFriends(const HeadNode&, const HeadNode&) const;
};


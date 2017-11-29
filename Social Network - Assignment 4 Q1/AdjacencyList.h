#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include <assert.h> 
#include <vector>
#include <stack>
#include <queue>

using namespace std;


class AdjacencyList
{
public:
	AdjacencyList(ifstream& file, size_t size = 30);
	static unique_ptr<AdjacencyList> getAdjacencyList(ifstream& file);
	
	void print() const;

	vector<string> getPersonLeastFriends() const;
	vector<string> getPersonMostFriends() const;
	vector<string> getPersonSpecificFriends(size_t n) const;

	void BFS(const string&, ofstream&) const;
	void DFS(const string&, ofstream&) const;

	void reset();

	pair<vector<string>, bool> findMutualFriends(const string&, const string&);

	~AdjacencyList();

private:

	struct Node {
		Node* next;
		string name;
		//bool isVisited = false;

		Node(const string& name = "") : next(nullptr), name(name) {}
		~Node() { delete next; }
		
		//void resetVisitedStatus() { isVisited = false; }
		//void visit() { isVisited = true; }
		size_t getFriends() const {
			Node* iter = next;
			size_t count = 0;

			while (iter) {
				iter = iter->next;
				++count;
			}
			return count;
		}
		void print() const {
			Node* iterator = next;
			while (iterator) {
				cout << iterator->name << ",";
				iterator = iterator->next;
			}
		}
		bool search(const string& name) const {
			Node* iterator = next;

			while (iterator) {
				if (iterator->name == name) return true;
				iterator = iterator->next;
			}
			return false;
		}
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
		}
	};

	pair<Node, bool>* profiles;
	size_t size;
	size_t currIndex = 0;

	void construct(ifstream& file);
	
	void insertPairOfNames(const pair<string, string>& names);
	const pair<string, string> readPairOfNames(ifstream& singeLine);
	
	size_t getMaxFriends() const;
	size_t getLeastFriends() const;

	void resize();
	
	bool areFriends(const string&, const string&) const ;
	pair<const Node*, const Node*> getFriendLists(const string& = "", const string& = "") const;
	vector<string> extractMutualFriends(const Node*, const Node*) const;
	pair<Node, bool>* getPair(const string& name) const ;
	pair<Node, bool>* getUnvisitedFriendPair(const pair<Node, bool>* name) const;

};


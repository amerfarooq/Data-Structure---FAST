#pragma once
#include "NAryTree.h"

using namespace std;

struct AVLNode {
	AVLNode* right;
	AVLNode* left;
	int height;
	char key;
	vector<pair<string, string>> files;	// pair<fileName, filePath>

	AVLNode(char key = '\0', string fileName = "", string filePath = "") 
		: right(nullptr), 
		  left(nullptr), 
		  height(0), 
		  key(key) 
	{
		insertFile(fileName, filePath);
	}

	void insertFile(string fileName, string filePath) {
		files.push_back(make_pair(fileName, filePath));
	}
	
	const vector<string> getPaths(const string fileName) const {
		vector<string> possiblePaths;
		
		for (const auto p : files) {
			if (p.first == fileName) {
				possiblePaths.push_back(p.second);
			}
		}
		return possiblePaths;
	}
	
	bool replacePath(const string originalPath, const string newPath) {
		for (auto& p : files) {
			if (p.second == originalPath) {
				p.second = newPath;
				return true;
			}
		}
		return false;
	}

	void deletePath(const string originalPath, const string fileName) {
		remove(files.begin(), files.end(), make_pair(fileName, originalPath));
	}

	void displayPaths() const {
		for (const auto& p : files) {
			cout << p.second << endl;
		}
	}
} ;

class AVLTree {

public:

	pair<vector<string>, bool> searchFile(string fileName) {
		char searchKey = tolower(fileName.at(0));
		AVLNode* foundNode = searchAVLNode(searchKey);
		
		if (foundNode) {
			vector<string> paths = foundNode->getPaths(fileName);
			
			if (paths.size() == 0) {
				return make_pair(vector<string>(), false);
			}
			return make_pair(paths, true);
		}
		else {
			return make_pair(vector<string>(), false);
		}
	}

	void insert(string fileName, string filePath) {
		char searchKey = tolower(fileName.at(0));
		AVLNode* foundNode = searchAVLNode(searchKey);
		
		if (!foundNode) {
			root = insert(searchKey, root, fileName, filePath);
		}
		else {
			foundNode->insertFile(fileName, filePath);
		}
	}

	void inOrder() {
		inOrder(root);
	}

	void preOrder() {
		preOrder(root);
	}

	void displayPaths() {
		displayPaths(root);
	}

	void replacePaths(const string originalPath, const string newPath, const string fileName) {
		AVLNode* pathNode = searchAVLNode(fileName.at(0));
		if (!pathNode->replacePath(originalPath, newPath))
			throw("Path could not be replaced!");
	}

	void removePath(const string originalPath, const string fileName) {
		AVLNode* pathNode = searchAVLNode(fileName.at(0));
		pathNode->deletePath(originalPath, fileName);
	}

	~AVLTree() {
		makeDeletion(root);
	}

private:

	AVLNode* root = nullptr;

	void remove(char key) {
		root = remove(root, key);
	}

	AVLNode* searchAVLNode(const char key) {
		AVLNode* iterator = root;
		
		while (iterator) {
			if (key < iterator->key) {
				iterator = iterator->left;
			}
			else if (key > iterator->key) {
				iterator = iterator->right;
			}
			else {
				return iterator;
			}
		}
		return nullptr;
	}
 
	AVLNode* leftRotation(AVLNode*& node) {
		AVLNode* newPosition = node->right;
		node->right = newPosition->left;
		newPosition->left = node;
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		newPosition->height = max(getHeight(newPosition->right), node->height) + 1;
		return newPosition;
	}

	AVLNode* rightRotation(AVLNode*& node) {
		AVLNode* newPosition = node->left;
		node->left = newPosition->right;
		newPosition->right = node;
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		newPosition->height = max(getHeight(newPosition->left), node->height) + 1;
		return newPosition;
	}

	AVLNode* leftRightRotation(AVLNode*& node) {
		node->right = rightRotation(node->right);
		return leftRotation(node);
	}

	AVLNode* rightLeftRotation(AVLNode*& node) {
		node->left = leftRotation(node->left);
		return rightRotation(node);
	}

	AVLNode* insert(char key, AVLNode*& node, string fileName, string filePath) {
		if (node == nullptr) {
			node = new AVLNode(key, fileName, filePath);
		}
		else if (key < node->key) {
			node->left = insert(key, node->left, fileName, filePath);

			if (getHeight(node->left) - getHeight(node->right) == 2) {
				if (key < node->left->key)
					node = rightRotation(node);
				else
					node = rightLeftRotation(node);
			}
		}
		else if (key > node->key) {
			node->right = insert(key, node->right, fileName, filePath);

			if (getHeight(node->right) - getHeight(node->left) == 2) {
				if (key > node->right->key)
					node = leftRotation(node);
				else
					node = leftRightRotation(node);
			}
		}
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		return node;
	}

	AVLNode* remove(AVLNode* node, char key)
	{
		if (!node) return node;

		if (key < node->key)
			node->left = remove(node->left, key);
		else if (key > node->key)
			node->right = remove(node->right, key);

		else {
			if ((!node->left ) || (!node->right)) {
				AVLNode* temp = node->left ? node->left : node->right;

				if (!temp){
					temp = node;
					node = nullptr;
				}
				else
					*node = *temp; 
								  
				delete(temp);
			}
			else {
				AVLNode* temp = findMin(node->right);

				node->key = temp->key;
				node->right = remove(node->right, temp->key);
			}
		}
		if (!node) return node;

		node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		int balance = getBalance(node);

		if (balance > 1 && getBalance(node->left) >= 0) {
			return rightRotation(node);
		}
		if (balance > 1 && getBalance(node->left) < 0) {
			node->left = leftRotation(node->left);
			return rightRotation(node);
		}
		if (balance < -1 && getBalance(node->right) <= 0) {
			return leftRotation(node);
		}
		if (balance < -1 && getBalance(node->right) > 0) {
			node->right = rightRotation(node->right);
			return leftRotation(node);
		}
		return node;
	}

	AVLNode* findMin(AVLNode* node)
	{
		if (!node) return nullptr;
		else if (!node->left) return node;
		else return findMin(node->left);
	}

	AVLNode* findMax(AVLNode* node)
	{
		if (!node) return nullptr;
		else if (!node->right) return node;
		else return findMax(node->right);
	}
	
	void inOrder(const AVLNode* node) {
		if (!node) return;
		else {
			inOrder(node->left);
			cout << node->key << endl;
			inOrder(node->right);
		}
	}

	void preOrder(const AVLNode* node) {
		if (!node) return;
		else {
			cout << node->key << endl;
			preOrder(node->left);
			preOrder(node->right);
		}
	}

	void displayPaths(const AVLNode* node) {
		if (!node) return;
		else {
			cout << node->key << endl;
			node->displayPaths();
			displayPaths(node->right);
			displayPaths(node->left);
		}
	}

	int getBalance(const AVLNode* node)
	{
		if (!node) return 0;
		else return getHeight(node->left) - getHeight(node->right);
	}

	int getHeight(const AVLNode* node) {
		return node == nullptr ? -1 : node->height;
	}

	void makeDeletion(AVLNode* node) {
		if (!node)
			return;
		else {
			makeDeletion(node->right);
			makeDeletion(node->left);
			delete node;
		}
	}
};
#pragma once

#include "windows.h" 
#include <conio.h> 
#include <iostream> 
#include <filesystem>
#include <string> 
#include <vector>
#include <queue>
#include "AVLTree.h"

using namespace std;
namespace fs = std::experimental::filesystem;

using dirEntry = fs::directory_entry;

class NAryTree {

public:

	NAryTree(string path, string fileName, AVLTree* avlTree) : avlTree(avlTree) {
		string folderPath = path + "\\" + fileName;
		root = new TreeNode(folderPath, fileName);
		avlTree->insert(fileName, folderPath);
		construct(root);
	}

	void levelOrder() {
		queue<reference_wrapper<TreeNode*>> toVisit;
		levelOrder(root, toVisit);
	}
	void postOrder() {
		postOrder(root);
	}
	void preOrder() {
		preOrder(root);
	}
	void treeOrder() {
		treeOrder(root);
	}

	const string getMainFolder() {
		return root->fileName;
	}

	void insert(vector<string> tokPath, string fileName) {
		TreeNode* insertHere = searchTreePath(tokPath);
		insertNode(fileName, insertHere);
	}

	void merge(vector<string>& mergee, vector<string>& mergeInto) {
		TreeNode* mergeeNode = searchTreePath(mergee);
		TreeNode* mergeeParent = searchTreePath(vector<string>(mergee.begin(), mergee.end() - 1));
		
		bool isChild = true;
		if (mergeeParent->child->fileName != mergee.at(mergee.size() - 1)) {
			TreeNode* iterator = mergeeParent->child;
			TreeNode* prev = nullptr;

			while (iterator) {
				if (iterator->fileName != mergee.at(mergee.size() - 1)) {
					prev = iterator;
					iterator = iterator->sibling;
				}
				else {
					mergeeParent = prev;
					isChild = false;
					break;
				}	
			}
		}
		TreeNode* mergeIntoNode = searchTreePath(mergeInto);
		mergeDirectories(mergeeNode, mergeeParent, isChild, mergeIntoNode);
	}

	void remove(vector<string> deletee) {
		if (deletee.size() == 1 && deletee.at(0) == getMainFolder()) {
			delete root;
			root = nullptr;
			return;
		}
			
		TreeNode* toBeDeleted = searchTreePath(deletee);
		TreeNode* toBeDeletedParent = searchTreePath(vector<string>(deletee.begin(), deletee.end() - 1));

		bool isChild = true;
		if (toBeDeletedParent->child->fileName != deletee.at(deletee.size() - 1)) {
			TreeNode* iterator = toBeDeletedParent->child;
			TreeNode* prev = nullptr;

			while (iterator) {
				if (iterator->fileName != deletee.at(deletee.size() - 1)) {
					prev = iterator;
					iterator = iterator->sibling;
				}
				else {
					toBeDeletedParent = prev;
					isChild = false;
					break;
				}
			}
		}
		removeNode(toBeDeleted, isChild, toBeDeletedParent);
	}

	uintmax_t getSize(vector<string> file) {
		TreeNode* fileNode = searchTreePath(file);
		return getSize(fileNode);
	}
	
	~NAryTree() {
		recursiveDelete(root);
		delete root;
		delete avlTree;
	}

private:

	struct TreeNode {
		TreeNode* child;
		TreeNode* sibling;
		string path;
		string fileName;

		TreeNode(string path, string fileName)
			: child(nullptr),
			sibling(nullptr),
			path(path),
			fileName(fileName) {}
	};

	TreeNode* root = nullptr;
	AVLTree* avlTree;

	void construct(TreeNode* node, bool isChild = true) {
		vector<TreeNode*> folders;
		TreeNode* temp = node;

		for (const auto& p : fs::directory_iterator(node->path)) {
			string filePath = p.path().string();
			string fileName = p.path().filename().string();
			avlTree->insert(fileName, filePath);

			TreeNode* newNode = new TreeNode(filePath, fileName);

			if (isChild) {
				node->child = newNode;
				node = newNode;
				isChild = false;
			}
			else {
				node->sibling = newNode;
				node = newNode;
			}
			if (isFolder(newNode)) folders.push_back(newNode);
		}
		for (auto& folder : folders)
			construct(folder);
	}

	void treeOrder(const TreeNode* node, int i = 0) {
		if (!node) return;

		cout << string(i, ' ') << node->fileName << endl;

		treeOrder(node->child, i + 2);
		treeOrder(node->sibling, i);
	}
	
	void postOrder(const TreeNode* node) {
		if (!node) return;

		postOrder(node->child);
		postOrder(node->sibling);

		cout << node->fileName << endl;
	}

	void preOrder(const TreeNode* node) {
		treeOrder(node);
	}

	void levelOrder(TreeNode* node, queue<reference_wrapper<TreeNode*>>& toVisit ) {
		if (!node) return;

		cout << node->fileName << endl;

		if (node->child)
			toVisit.push(node->child);

		levelOrder(node->sibling, toVisit);

		for (int i = 0; i < toVisit.size(); ++i) {
			if (toVisit.front() == node) {
				toVisit.pop();
			}
			TreeNode*& visitee = toVisit.front();
			toVisit.pop();
			cout << endl;
			levelOrder(visitee, toVisit);
		}
	}

	void mergeDirectories(TreeNode* mergee, TreeNode* mergeeParent, bool isChild, TreeNode* mergeInto) {
		// If mergeInto has a child, find that child's last sibling and make it a sibling of mergee->child
		if (mergeInto->child) {
			TreeNode* iterator = mergeInto->child;
			TreeNode* prev = nullptr;

			while (iterator) {
				prev = iterator;
				iterator = iterator->sibling;
			}
			prev->sibling = mergee->child;
		}
		else {
			mergeInto->child = mergee->child;
		}
		// Adjust paths of merged files/folders
		adjustAllPaths(mergee->child, mergee->path, mergeInto->path);
		
		// Adjust parent node of mergee
		if (mergee->sibling) {
			if (isChild)
				mergeeParent->child = mergee->sibling;
			else {
				mergeeParent->sibling = mergee->sibling;
			}
		}
		else {
			if (isChild)
				mergeeParent->child = nullptr;
			else {
				mergeeParent->sibling = nullptr;
			}
		}
	}

	void recursiveDelete(TreeNode*& node) {
		if (!node) {
			return;
		}
		else if (isFolder(node)) {
			avlTree->removePath(node->path, node->fileName);
			recursiveDelete(node->child);
			recursiveDelete(node->sibling);
			delete node;
		}
		else {
			avlTree->removePath(node->path, node->fileName);
			recursiveDelete(node->sibling);
			delete node;
		}
	}

	void removeNode(TreeNode* node, bool isChild, TreeNode* nodeParent) {
		if (isFolder(node)) {
			recursiveDelete(node->child);
		}
		if (node->sibling) {
			if (isChild) {
				nodeParent->child = node->sibling;
			}
			else {
				nodeParent->sibling = node->sibling;
			}
		}
		else {
			if (isChild)
				nodeParent->child = nullptr;
			else
				nodeParent->sibling = nullptr;
		}
		avlTree->removePath(node->path, node->fileName);
		delete node;
	}

	uintmax_t getFileSize(const TreeNode* node) {
		return fs::file_size(fs::v1::path(node->path));
	}

	uintmax_t getDirectorySize(const TreeNode* node) {
		if (!node) {
			return {};
		}
		else if (isFolder(node)) {
			return getDirectorySize(node->child) + getDirectorySize(node->sibling);
		}
		else {
			return getFileSize(node) + getDirectorySize(node->sibling);
		}
	}

	uintmax_t getSize(const TreeNode* node) {
		if (isFolder(node))
			return getDirectorySize(node->child);
		else
			return getFileSize(node);
	}

	void searchTreeFileName(TreeNode*& node, const string fileName, const bool isDir, vector<reference_wrapper<TreeNode*>>& foundNodes) {
		if (!node) return;

		if (isFolder(node)) {
			if (isDir && node->fileName == fileName)
				foundNodes.push_back(node);

			searchTreeFileName(node->child, fileName, isDir, foundNodes);
			searchTreeFileName(node->sibling, fileName, isDir, foundNodes);
		}
		else {
			if (!isDir && node->fileName == fileName)
				foundNodes.push_back(node);

			searchTreeFileName(node->sibling, fileName, isDir, foundNodes);
		}
	}

	TreeNode* findNodePtr(TreeNode* node, const string file) {
		if (node->fileName == file) 
			return node;
		else {
			TreeNode* iterator = node->sibling;
			while (iterator) {
				if (iterator->fileName == file) 
					return iterator;
				else 
					iterator = iterator->sibling;
			}
		}
	}

	TreeNode*& findNodeAtDepthRef(TreeNode* node, const string file) {
		if (node->fileName == file) 
			return node;
		else {
			TreeNode* iterator = node->sibling;
			TreeNode* prev = node;
			while (iterator) {
				if (iterator->fileName == file) 
					return prev->sibling;
				else {
					prev = iterator;
					iterator = iterator->sibling;
				}
			}
			throw("Node not found. Cannot return null reference.");
		}
	}

	TreeNode* getDirectoryNode(TreeNode* node, const string file) {
		if (node->fileName == file) 
			return node;
		else {
			TreeNode* iterator = node->sibling;
			while (iterator) {
				if (iterator->fileName == file) return iterator;
				else iterator = iterator->sibling;
			}
		}
	}

	TreeNode* searchTreePath(vector<string> dirs) {
		if (dirs.size() == 0) {
			throw("Directory vector is empty");
		}
		TreeNode* foundNode = root;

		for (int i = 0; i < dirs.size(); ++i) {
			if (dirs.size() - i != 1) {
				foundNode = findNodePtr(foundNode, dirs.at(i))->child;
			}
			else {
				return findNodeAtDepthRef(foundNode, dirs.at(i));
			}
		}
	}

	TreeNode*& findNodeRef(TreeNode* node, const string fileName) {
		if (node == nullptr) throw("Node should not be null");
		
		if (node->fileName == fileName) {
			return node;
		}
		else {
			findNodeRef(node->sibling, fileName);
		}
	}

	TreeNode*& getNodeRef(vector<string>dirs) {
		if (dirs.size() == 0) {
			throw("Directory vector is empty");
		}
		TreeNode* foundNode = root;
		TreeNode* prev = nullptr;

		for (int i = 0; i < dirs.size(); ++i) {
			if (dirs.size() - i != 1) {
				prev = foundNode;
				foundNode = findNodePtr(foundNode, dirs.at(i))->child;
			}
			else {
				string lastFileName = dirs.at(dirs.size() - 1);
				
				if (foundNode->fileName == lastFileName) {
					if (foundNode == root) {
						if (!root->sibling) return root->sibling;
						else return findNodeRef(root->sibling, lastFileName);
					}
					return prev->child;
				}
				return findNodeRef(foundNode, lastFileName);
			}
		}
	}

	void insertNode(const string fileName, TreeNode*& insertAt) {
		TreeNode* newNode = new TreeNode(insertAt->path + "\\" + fileName, fileName);

		if (insertAt->child) {
			TreeNode* iterator = insertAt->child;

			while (iterator->sibling) {
				iterator = iterator->sibling;
			}
			iterator->sibling = newNode;
		}
		else {
			insertAt->child = newNode;
		}
	}

	bool isFolder(const TreeNode* node) {
		return node->path.find("\.txt") == string::npos;
	}

	string replace(string& str, const string& from, const string& to) {
		size_t start_pos = str.find(from);
		return str.replace(start_pos, from.length(), to);
	}

	void adjustAllPaths(TreeNode* toAdjust, string originalPath, string newPath) {
		if (toAdjust == nullptr)
			return;
		else {
			string origPath = toAdjust->path;
			string replacedPath = replace(toAdjust->path, originalPath, newPath);
			avlTree->replacePaths(origPath, replacedPath, toAdjust->fileName);
			adjustAllPaths(toAdjust->sibling, originalPath, newPath);
			adjustAllPaths(toAdjust->child, originalPath, newPath);
		}
	}
	
};

#pragma once
#include "AVLTree.h"
#include "StringFunctions.h"
#include <iomanip> 

using namespace std;

class Menu {
	
public:
	Menu(AVLTree* avlTree, NAryTree* nTree) : naryTree(nTree), avlTree(avlTree) {}
	
	void insert() {
		naryTree->treeOrder();

		string folder;
		cout << "\nEnter directory where new folder/file is to be added: ";
		cin >> folder;

		string newFolderName;
		cout << "Enter name of new folder/file: ";
		cin >> newFolderName;

		pair<vector<string>, bool> searchedNode = avlTree->searchFile(folder);
		if (!searchedNode.second) {
			cout << "\nDirectory does not exist!" << endl;
			return;
		}
		else {
			string folderPath;
			if (searchedNode.first.size() > 1) {
				for (size_t i = 0; i < searchedNode.first.size(); i++) {
					cout << "\n" << i + 1 << ":" << endl << " " << searchedNode.first.at(i) << endl;
				}
				int pathIndex;
				cout << "\nPlease enter the index of your desired path: ";
				cin >> pathIndex;
				
				folderPath = searchedNode.first.at(pathIndex - 1);
			}
			else if (searchedNode.first.size() == 1){
				folderPath = searchedNode.first.at(0);
			}
			else {
				throw("Cannot be less than one here");
			}
			vector<string> tokenizedPath = tokenizePath(folderPath, naryTree->getMainFolder());
			naryTree->insert(tokenizedPath, newFolderName);
			avlTree->insert(newFolderName, folderPath + "\\" + newFolderName);
			naryTree->treeOrder();
			cout << "\nFile/Folder insert succesfully!";
		}
	}
	
	void mergeFolders() {
		naryTree->treeOrder();

		string mergee, mergeePath;
		cout << "\nEnter folder to merge: ";
		cin >> mergee;

		string mergeInto, mergeIntoPath;
		cout << "Enter folder to merge into: ";
		cin >> mergeInto;

		pair<vector<string>, bool> mergeePaths = avlTree->searchFile(mergee);
		if (!mergeePaths.second) {
			cout << "\nInvalid directory!" << endl;
			return;
		}
		else {
			if (mergeePaths.first.size() > 1) {
				cout << endl;

				for (int i = 0; i < mergeePaths.first.size(); ++i) {
					cout << "\n" << i + 1 << ":" << endl << " " << mergeePaths.first.at(i) << endl;
				}
				int pathIndex;
				cout << "\nEnter index of desired path: ";
				cin >> pathIndex;
				mergeePath = mergeePaths.first.at(pathIndex - 1);
			}
			else {
				mergeePath = mergeePaths.first.at(0);
			}
		}

		pair<vector<string>, bool> mergeIntoPaths = avlTree->searchFile(mergeInto);
		if (!mergeIntoPaths.second) {
			cout << "\nInvalid directory!" << endl;
			return;
		}
		else {
			if (mergeIntoPaths.first.size() > 1) {
				cout << endl;

				for (int i = 0; i < mergeIntoPaths.first.size(); ++i) {
					cout << "\n" << i + 1 << ":" << endl << " " << mergeIntoPaths.first.at(i) << endl;
				}
				int pathIndex;
				cout << "\nEnter index of desired path: ";
				cin >> pathIndex;
				mergeIntoPath = mergeIntoPaths.first.at(pathIndex - 1);
			}
			else {
				mergeIntoPath = mergeIntoPaths.first.at(0);
			}
		}
		if (mergeePath == mergeIntoPath) {
			cout << "\nCannot merge a file into itself!" << endl;
			return;
		}
		if (!fs::is_directory(fs::path(mergeePath)) || !fs::is_directory(fs::path(mergeIntoPath))) {
			cout << "\nOnly folders can be merged!" << endl;
			return;
		}
		if (comparePaths(mergeePath, mergeIntoPath, naryTree->getMainFolder())) {
			naryTree->merge(tokenizePath(mergeePath, naryTree->getMainFolder()),
							tokenizePath(mergeIntoPath, naryTree->getMainFolder()));
		}
		else {
			cout << "\nInvalid directories!" << endl;
			return;
		}
		naryTree->treeOrder();
		cout << "\nFolders merged succesfully!";
	}

	void remove() {
		naryTree->treeOrder();

		string deletee;
		cout << "\nEnter name of folder/file to be deleted: ";
		cin >> deletee;

		pair<vector<string>, bool> searchedNode = avlTree->searchFile(deletee);
		if (!searchedNode.second) {
			cout << "\nDirectory does not exist!" << endl;
			return;
		}
		else {
			string folderPath;
			if (searchedNode.first.size() > 1) {
				for (size_t i = 0; i < searchedNode.first.size(); i++) {
					cout << "\n" << i + 1 << ":" << endl << " " << searchedNode.first.at(i) << endl;
				}
				int pathIndex;
				cout << "\nPlease enter the index of your desired path: ";
				cin >> pathIndex;

				folderPath = searchedNode.first.at(pathIndex - 1);
			}
			else if (searchedNode.first.size() == 1) {
				folderPath = searchedNode.first.at(0);
			}
			else {
				throw("Cannot be less than one here");
			}
			vector<string> tokenizedPath = tokenizePath(folderPath, naryTree->getMainFolder());
			naryTree->remove(tokenizedPath);
		}
		cout << endl;
		naryTree->treeOrder();
		cout << "\nFile/Folder removed succesfully!";
	}

	void search() {
		naryTree->treeOrder();

		string toSearch;
		cout << "\nEnter name of folder/file to be searched: ";
		cin >> toSearch;

		pair<vector<string>, bool> searchedNode = avlTree->searchFile(toSearch);
		
		if (searchedNode.second) {
			cout << "\nFollowing files found: " << endl << endl;

			for (int i = 0; i < searchedNode.first.size(); ++i) {
				cout << i + 1 << ":\n " << searchedNode.first.at(i) << endl << endl;
			}
			return;
		}
		else {
			cout << "\nInvalid file/folder name!" << endl;
		}
	}

	void getSize() {
		naryTree->treeOrder();

		string getSizeOf;
		cout << "\nEnter name of folder/file to get size of: ";
		cin >> getSizeOf;
		cout << endl;

		pair<vector<string>, bool> searchedNode = avlTree->searchFile(getSizeOf);
		if (!searchedNode.second) {
			cout << "Directory does not exist!" << endl;
			return;
		}
		else {
			string folderPath;
			if (searchedNode.first.size() > 1) {
				for (size_t i = 0; i < searchedNode.first.size(); i++) {
					cout << "\n" << i + 1 << ":" << endl << " " << searchedNode.first.at(i) << endl;
				}
				int pathIndex;
				cout << "\nPlease enter the index of your desired path: ";
				cin >> pathIndex;

				folderPath = searchedNode.first.at(pathIndex - 1);
			}
			else if (searchedNode.first.size() == 1) {
				folderPath = searchedNode.first.at(0);
			}
			else {
				throw("Cannot be less than one here");
			}
			vector<string> tokenizedPath = tokenizePath(folderPath, naryTree->getMainFolder());
			cout << setprecision(2) << "\nSize: " << naryTree->getSize(tokenizedPath) / 1024.0 << " Kb" << endl;
		}
	}

	void display() {
		vector<string> displayOptions = { "Tree View", "Level Order", "Post-Order", "Pre-Order" };
		
		for (size_t i = 0; i < displayOptions.size(); i++) {
			cout << i + 1 << ": " << displayOptions.at(i) << endl;
		}
		int index;
		cout << "\nEnter index of desired display option: ";
		cin >> index;

		if (index < 1 || index > displayOptions.size()) {
			cout << "\nInvalid Option" << endl;
			return;
		}
		else {
			cout << endl << endl;
			switch (index) {
				
				case 1:
					naryTree->treeOrder();
					return;
				case 2:
					naryTree->levelOrder();
					return;
				case 3:
					naryTree->postOrder();
					return;
				case 4:
					naryTree->preOrder();
					return;
			}
		}
	}

private:
	NAryTree* naryTree = nullptr;
	AVLTree* avlTree = nullptr;
};

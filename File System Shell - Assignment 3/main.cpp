#include "NAryTree.h"
#include "Menu.h"
#include <chrono>
#include <thread>


int main() {
	
	AVLTree* avlTree = new AVLTree;
	NAryTree* naryTree = new NAryTree(getCurrentDirectory(), "logs", avlTree);
	Menu mainMenu(avlTree, naryTree); 
	
	while (true) {
		vector<string> operations = { "Display file/folder",
									  "Search file/folder"	,
									  "Merge folders",
									  "Add file/folder",
									  "Remove file/folder",
									  "Get size of file/folder",
									  "Exit"
									};
		cout << "\n\n---------------------" << endl;
		cout << "|  Filesystem Shell |" << endl;
		cout << "---------------------" << endl << endl;

		for (int i = 0; i < operations.size(); ++i) {
			cout << i + 1 << " - " << operations.at(i) << endl;
		}
		cout << "\nEnter operation number: ";
		int op; cin >> op;
		cout << endl;
		
		if (op < 1 || op > operations.size()) {
			cout << "\nInvalid Option!" << endl;
			continue;
		}
		else {
			switch (op) {
				case 1:
					mainMenu.display();
					break;
				case 2:
					mainMenu.search();
					break;
				case 3:
					mainMenu.mergeFolders();
					break;
				case 4:
					mainMenu.insert();
					break;
				case 5:
					mainMenu.remove();
					break;
				case 6:
					mainMenu.getSize();
					break;
				case 7:
					return 0;
			}
		}
		std::this_thread::sleep_for(1s);
	}
}








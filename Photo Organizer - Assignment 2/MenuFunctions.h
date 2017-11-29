#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <windows.h>
#include <string.h>
#include "PhotoCategory.h"
#include "PhotoNode.h"
#include "AuxNode.h"
#include "OperationStack.h"

using namespace std;

OperationStack undoStack;

// Returns a string containing the file path of the current directory.
string getCurrentDirectory() {
	char directory[MAX_PATH + 1];
	GetCurrentDirectoryA(sizeof(directory), directory);
	return directory;
}

// Adds a photo to a selection of categories.
void addPhoto(
	PhotoCategory* categories,
	int size,
	const int categoryCount,
	const string title,
	const string name,
	const string path,
	const string date
	) {

	PhotoNode* newPhoto = new PhotoNode(title, name, path , date );

	cout << "\nSelect the categories for the photo. Enter 0 to exit: " << endl;
	for (int i = 1; i < categoryCount; ++i) {
		cout << i << "- " << categories[i].getName() << endl;
	}
	cout << endl;
	cout << "Selected Categories:" << endl;

	categories[0].append(newPhoto); // Every photo is added to the default category.
									// TODO: Implement functionality to ensure same input choice is not entered again.
	int inputChoice;
	while (true) {
		cin >> inputChoice;
		if (inputChoice == 0) break;
		categories[inputChoice].append(newPhoto);
	}
}

// Deletes a photo from all the categories it belongs to.
void deletePhoto(
	PhotoCategory* categories,
	const int size,
	const int categoryCount,
	const string title,
	const string fileName
	) {
	PhotoNode* iterator = categories[0].start;   // Iterator is set to the start of the Default category.
	while (iterator != nullptr) {

		if (iterator->fileName == fileName && iterator->title == title) {

			AuxNode* auxNodeIter = iterator->start;
			while (auxNodeIter != nullptr) {

				// If the PhotoNode is the first node in the PhotoCategory.
				if (auxNodeIter->prev == nullptr) {
					for (int i = 0; i < categoryCount; ++i) {
						if (categories[i].getName() == auxNodeIter->category) {
							categories[i].start = auxNodeIter->target;
							if (auxNodeIter->target != nullptr)
								auxNodeIter->target->getCategoryAuxNode(auxNodeIter->category)->prev = nullptr;
							break;
						}
					}
				}
				else {
					// Adjust target and prev pointers.
					auxNodeIter->prev->getCategoryAuxNode(auxNodeIter->category)->target = auxNodeIter->target;
					if (auxNodeIter->target != nullptr)
						auxNodeIter->target->getCategoryAuxNode(auxNodeIter->category)->prev = auxNodeIter->prev;
				}
				auxNodeIter = auxNodeIter->next;
			}
			break;
		}
		iterator = iterator->getCategoryTarget(categories[0].getName());
	}
	if (iterator != nullptr) {
		undoStack.push(new UndoDeletePhoto(iterator));
		delete iterator;
		cout << "\nPhoto deleted successfully!" << endl;
		return;
	}
	else {
		cout << "\nPhoto not found!" << endl;
	}
}

// Adds a category.
void addCategory(
	PhotoCategory*& categories,
	int& size, int& categoryCount,
	string name
	) {
	// Check if categories array is full.
	if (categoryCount != size) {
		categories[categoryCount++].setName(name);
	}
	// If array is full, then double the size of the array and copy over the previous categories.
	else {
		size *= 2;
		PhotoCategory* newCategories = new PhotoCategory[size];
		for (int i = 0; i < categoryCount; ++i) {
			newCategories[i] = categories[i];
		}
		newCategories[categoryCount++].setName(name);
		delete[] categories;
		categories = newCategories;
	}
}

// Deletes a category.
void deleteCategory(
	PhotoCategory*& categories,
	const int size,
	int& categoryCount,
	string categoryToDelete
	) {

	// Check if category name is valid
	for (int i = 0; i < categoryCount; ++i) {
		if (categories[i].getName() == categoryToDelete) {
			UndoDeleteCategory* delCategoryOp = new UndoDeleteCategory(categories[i]);
			undoStack.push(delCategoryOp);

			categories[i].deleteCategory();
			categories[0].checkDefaultCategory();

			int j = i;
			for (; j < categoryCount - 1; ++j) {
				categories[j] = categories[j + 1];
			}
			categories[j + 1].setName("");
			categories[j + 1].start = nullptr;
			categories[j + 1].totalPhotos = 0;
			--categoryCount;
			break;
		}
	}
}

// Adds a photo to a specific category.
void addPhotoCategory(
	PhotoCategory*& categories,
	const int size,
	const int categoryCount,
	const string title,
	const string fileName
	) {
	PhotoNode* iterator = categories[0].start;   // Iterator is set to the start of the Default category.
	while (iterator != nullptr) {

		if (iterator->fileName == fileName && iterator->title == title) {
			// If photo is found, display list of categories and ask user to pick one.
			cout << "\n---Available Categories---" << endl;
			for (int i = 1; i < categoryCount; i++)
			{
				cout << i << "- " << categories[i].getName() << endl;
			}
			cout << endl;

			while (true) {
				int newCategoryIndex;
				cout << "\nSelect category: ";
				cin >> newCategoryIndex;

				if (newCategoryIndex < 1 || newCategoryIndex >= categoryCount) {
					cout << "Invalid Option! Try again" << endl;
				}
				else {
					while (true) {
						// Check if photo is already in given category.
						if (iterator->isThisCategoryInPhoto(categories[newCategoryIndex].getName())) {
							cout << "Photo is already in this category!" << endl;
							break;
						}
						else {
							categories[newCategoryIndex].append(iterator);
							return;
						}
					}
				}
			}
		}
		iterator = iterator->getCategoryTarget(categories[0].getName());
	}
	cout << "\nPhoto not found!" << endl;
}

// Removes a photo from a specific category.
void deletePhotoCategory(
	PhotoCategory*& categories,
	const int size,
	const int categoryCount,
	const string title,
	const string fileName
	) {

	PhotoNode*& iterator = categories[0].start;   // Iterator only traverses the default category.
	while (iterator != nullptr) {

		if (iterator->fileName == fileName && iterator->title == title) {
			// If photo is found, display a list of categories to which it belongs.
			cout << "\nThis photo belongs to the following categories: " << endl;
			for (int i = 1; i < iterator->categoryCount; ++i) {
				cout << i << "- " << iterator->categories[i] << endl;
			}
			cout << endl;

			while (true) {
				// Ask user to enter the index of the category he wants to delete from the photo.
				int categoryIndex;
				cout << "\nEnter index of category: ";
				cin >> categoryIndex;

				if (categoryIndex < 1 || categoryIndex >= iterator->categoryCount) {
					cout << "Invalid Option! Try again" << endl;
				}
				else {
					while (true) {
						string categoryToDelete = iterator->categories[categoryIndex];
						// Check if photo is not in the given category.
						//if (!iterator->isThisCategoryInPhoto(categoryToDelete)) {
						//	cout << "Photo is not in this category!" << endl;
						//	break;
						//}
						//else {
						AuxNode* aux = iterator->getCategoryAuxNode(categoryToDelete);
						// Check if PhotoNode is the first node of the to-delete category.
						if (aux->prev == nullptr) {
							for (int i = 0; i < categoryCount; ++i) {
								if (categories[i].getName() == categoryToDelete) {
									categories[i].start = aux->target;
								}
							}
						}
						// If it is not, then go the previous PhotoNode of the same category, extract
						// its AuxNode and set its target to aux's target.
						else {
							aux->prev->getCategoryAuxNode(categoryToDelete)->target = aux->target;
						}
						iterator->deleteCategoryAuxNode(categoryToDelete);
						return;
						//} // closing of commented else above
					}
				}
			}
		}
		iterator = iterator->getCategoryTarget(categories[0].getName());
	}
	cout << "\nPhoto not found!" << endl;
}

// Returns a pair containing an array of integers that represent categories selected by the user and an intger 
// representing the total number of such selections.
pair<int*, int> getCategoryInput(
	const PhotoCategory* categories,
	const int size,
	const int categoryCount
	) {
	cout << "\n|---Categories---| " << endl << endl;
	for (size_t i = 1; i < categoryCount; ++i) {
		cout << i << "- " << categories[i].getName() << endl;
	}
	cout << endl << "Enter the index of the category. To end selection, enter 0." << endl;

	int* selectedCategories = new int[categoryCount - 1];
	int optionCount = 0;
	int option;
	int totalOptionsEntered = 0;
	while (true) {

		cin >> option;

		if (option == 0) {
			if (totalOptionsEntered == 0 || totalOptionsEntered >= 2)
				break;
			else if (totalOptionsEntered == 1)
				cout << "Atleast 2 categories must be selected!" << endl;
		}
		else if (option < 0 || option >= categoryCount)
			cout << "Invalid index!" << endl;
		else if (optionCount == 0) {
			selectedCategories[optionCount++] = option;
			++totalOptionsEntered;
		}
		else {
			int* ptr = find(selectedCategories, selectedCategories + categoryCount - 1, option);
			if (ptr == selectedCategories + categoryCount - 1) {
				selectedCategories[optionCount++] = option;
				++totalOptionsEntered;
				continue;
			}
			else 
				cout << "Index already entered before!" << endl;
		}
	}
	pair<int*, int> newPair;
	newPair.first = selectedCategories;
	newPair.second = optionCount;
	return newPair;
}

// Displays given html string on browser using ShellExecute
void displayInBrowser(const string html) {
	fstream fout;
	fout.open("file.html", ios::out | ios::trunc);
	fout << html;
	fout.close();

	string link = getCurrentDirectory() + "\\file.html";
	ShellExecute(NULL, "open", link.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}

// Displays a particular category.
void displayCategory(
	const PhotoCategory* const categories,
	const int size,
	const int categoryCount
	) {
	cout << "\n|---Categories---| " << endl << endl;
	for (size_t i = 1; i < categoryCount; ++i) {
		cout << i << "- " << categories[i].getName() << endl;
	}

	bool arePhotosAppended = false;
	cout << endl << "Enter the index of the category: ";
	int category;

	while (true) {
		cin >> category;
		if (category < 1 || category >= categoryCount) {
			cout << "Invalid option! Try again" << endl;
			continue;
		}
		else {
			break;
		}
	}
	string html = "<html><head><title>Photo Organizer</title><style type=\"text/css\">h1 {font-size: 42px; font-family: Arial}</style></head><body style=""background-color:#F9E79F;""><h1 align = 'center'>" + categories[category].getName() + "</h1>";

	PhotoNode* iterator = categories[category].start;
	while (iterator) {
		html += "<center><img src='" + iterator->filePath + "\\" + iterator->fileName + "' width=300px height=300px></center>";
		html += "<p align='center' style=""font-size:18 %;""><font size='5' face='arial'><b>" + iterator->title + " - " + iterator->dateOfCreation + "</font></b></p>";
		arePhotosAppended = true;
		iterator = iterator->getCategoryTarget(categories[category].getName());
	}

	if (!arePhotosAppended) {
		html += "<br></br><br></br><p align='center' style=""font-size:18 %;""><font size='5' face='arial'>There doesn't seem to be anything here!</p>";
	}
	html += "</body></html>";
	displayInBrowser(html);
}

// Displays union of multiple categories.
void displayUnion(
	const PhotoCategory* const categories,
	const int size,
	const int categoryCount
	) {
	pair<int*, int> categoriesAndSize = getCategoryInput(categories, size, categoryCount);
	int* selectedCategories = categoriesAndSize.first;
	int optionCount = categoriesAndSize.second;

	// In case no categories are selected.
	if (optionCount == 0) return;

	string html = "<html><head><title>Photo Organizer</title><style type=\"text/css\">h1 {font-size: 42px; font-family: Arial}</style></head><body style=""background-color:#F9E79F;""><h1 align = 'center'> Displaying union of following categories</h1>";
	for (int i = 0; i < optionCount; ++i) {
		html += "<p align='center' style=""font-size:23 %;""><font size='6' face='arial'>" + categories[selectedCategories[i]].getName() + "</p>";
	}

	PhotoNode* iterator = categories[0].start;
	bool arePhotosAppended = false;
	while (iterator) {
		for (int i = 0; i < optionCount; ++i) {
			if (iterator->isThisCategoryInPhoto(categories[selectedCategories[i]].getName())) {
				html += "<center><img src='" + iterator->filePath + "\\" + iterator->fileName + "' width=300px height=300px></center>";
				html += "<p align='center' style=""font-size:18 %;""><font size='5' face='arial'><b>" + iterator->title + " - " + iterator->dateOfCreation + "</font></b></p>";
				iterator = iterator->getCategoryTarget(categories[0].getName());
				arePhotosAppended = true;
				break;
			}
		}
	}
	if (!arePhotosAppended) {
		html += "<br></br><br></br><p align='center' style=""font-size:18 %;""><font size='5' face='arial'>There doesn't seem to be anything here!</p>";
	}
	html += "</body></html>";
	displayInBrowser(html);
}

// Displays intersection of multiple categories.
void displayIntersection(
	const PhotoCategory* const categories,
	const int size,
	const int categoryCount) {
	pair<int*, int> categoriesAndSize = getCategoryInput(categories, size, categoryCount);
	int* selectedCategories = categoriesAndSize.first;
	int optionCount = categoriesAndSize.second;

	// In case no categories are selected.
	if (optionCount == 0) return;

	string html = "<html><head><title>Photo Organizer</title><style type=\"text/css\">h1 {font-size: 42px; font-family: Arial}</style></head><body style=""background-color:#F9E79F;""><h1 align = 'center'> Displaying intersection of following categories</h1>";
	for (int i = 0; i < optionCount; ++i) {
		html += "<p align='center' style=""font-size:23 %;""><font size='6' face='arial'>" + categories[selectedCategories[i]].getName() + "</p>";
	}

	PhotoNode* iterator = categories[0].start;
	bool arePhotosAppended = false;
	while (iterator) {
		bool inAllCategories = true;
		for (int i = 0; i < optionCount; ++i) {
			if (!iterator->isThisCategoryInPhoto(categories[selectedCategories[i]].getName())) {
				inAllCategories = false;
			}
		}
		if (inAllCategories) {
			html += "<center><img src='" + iterator->filePath + "\\" + iterator->fileName + "' width=300px height=300px></center>";
			html += "<p align='center' style=""font-size:18 %;""><font size='5' face='arial'><b>" + iterator->title + " - " + iterator->dateOfCreation + "</font></b></p>";
			arePhotosAppended = true;
		}
		iterator = iterator->getCategoryTarget(categories[0].getName());
	}
	if (!arePhotosAppended) {
		html += "<br></br><br></br><p align='center' style=""font-size:18 %;""><font size='5' face='arial'>There doesn't seem to be anything here!</p>";
	}

	html += "</body></html>";
	displayInBrowser(html);
}

// Displays XOR of two categories.
void displayXor(
	const PhotoCategory* const categories,
	const int size,
	const int categoryCount
	) {
	pair<int*, int> categoriesAndSize = getCategoryInput(categories, size, categoryCount);
	int* selectedCategories = categoriesAndSize.first;
	int optionCount = categoriesAndSize.second;

	// In case no categories are selected.
	if (optionCount == 0) return;

	string html = "<html><head><title>Photo Organizer</title><style type=\"text/css\">h1 {font-size: 42px; font-family: Arial}</style></head><body style=""background-color:#F9E79F;""><h1 align = 'center'> Displaying XOR of following categories</h1>";
	for (int i = 0; i < optionCount; ++i) {
		html += "<p align='center' style=""font-size:23 %;""><font size='6' face='arial'>" + categories[selectedCategories[i]].getName() + "</p>";
	}
	PhotoNode* iterator = categories[0].start;
	bool arePhotosAppended = false;
	while (iterator) {
		int xorCounter = 0;		// XOR means photo is only in category. xorCounter is used to track this.
		for (int i = 0; i < optionCount; ++i) {
			if (iterator->isThisCategoryInPhoto(categories[selectedCategories[i]].getName())) {
				++xorCounter;
			}
		}
		if (xorCounter == 1) {
			html += "<center><img src='" + iterator->filePath + "\\" + iterator->fileName + "' width=300px height=300px></center>";
			html += "<p align='center' style=""font-size:18 %;""><font size='5' face='arial'><b>" + iterator->title + " - " + iterator->dateOfCreation + "</font></b></p>";
			arePhotosAppended = true;
		}
		iterator = iterator->getCategoryTarget(categories[0].getName());
	}
	if (!arePhotosAppended) {
		html += "<br></br><br></br><p align='center' style=""font-size:18 %;""><font size='5' face='arial'>There doesn't seem to be anything here!</p>";
	}
	html += "</body></html>";
	displayInBrowser(html);
}

// Displays photos with a specific title
void displayTitleWise(
	const PhotoCategory* const categories,
	const int size,
	const int categoryCount) {
	string title;
	cout << "\nEnter title: ";
	cin >> title;

	string html = "<html><head><title>Photo Organizer</title><style type=\"text/css\">h1 {font-size: 42px; font-family: Arial}</style></head><body style=""background-color:#F9E79F;""><h1 align = 'center'> Displaying photos titled " + title + "</h1>";
	PhotoNode* iterator = categories[0].start;
	bool arePhotosAppended = false;
	while (iterator) {
		if (iterator->title == title) {
			html += "<center><img src='" + iterator->filePath + "\\" + iterator->fileName + "' width=300px height=300px></center>";
			html += "<p align='center' style=""font-size:18 %;""><font size='5' face='arial'><b>" + iterator->title + " - " + iterator->dateOfCreation + "</font></b></p>";
			arePhotosAppended = true;
		}
		iterator = iterator->getCategoryTarget(categories[0].getName());
	}
	if (!arePhotosAppended) {
		html += "<br></br><br></br><p align='center' style=""font-size:18 %;""><font size='5' face='arial'>There doesn't seem to be anything here!</p>";
	}
	html += "</body></html>";
	displayInBrowser(html);

}

// Displays photos taken on a specific date
void displayDateWise(
	const PhotoCategory* const categories,
	const int size,
	const int categoryCount) {
	cout << "\nEnter date (DD/MM/YY): ";
	string date;
	while (true) {
		cin >> date;
		if (!cin || date.length() != 8) {
			cout << "Invalid Date! Try again." << endl;
			continue;
		}
		else {
			break;
		}
	}
	string html = "<html><head><title>Photo Organizer</title><style type=\"text/css\">h1 {font-size: 42px; font-family: Arial}</style></head><body style=""background-color:#F9E79F;""><h1 align = 'center'> Displaying photos taken on " + date + "</h1>";
	PhotoNode* iterator = categories[0].start;
	bool arePhotosAppended = false;
	while (iterator) {
		if (iterator->dateOfCreation == date) {
			html += "<center><img src='" + iterator->filePath + "\\" + iterator->fileName + "' width=300px height=300px></center>";
			html += "<p align='center' style=""font-size:18 %;""><font size='5' face='arial'><b>" + iterator->title + " - " + iterator->dateOfCreation + "</font></b></p>";
			arePhotosAppended = true;
		}
		iterator = iterator->getCategoryTarget(categories[0].getName());
	}
	if (!arePhotosAppended) {
		html += "<br></br><br></br><p align='center' style=""font-size:18 %;""><font size='5' face='arial'>There doesn't seem to be anything here!</p>";
	}
	html += "</body></html>";
	displayInBrowser(html);

}

// Restore a deleted photo.
void undo(
	PhotoCategory*& categories, 
	int& size, 
	int& categoryCount)
	{
	UndoOperation* operation = undoStack.pop();
	
	if (operation == nullptr) {
		cout << "\nNo Undo to perform!" << endl;
		return;
	}


	if (operation->getOperationName() == deletePhotoOp) {
		static_cast<UndoDeletePhoto*>(operation)->undo(categories, size, categoryCount);
		cout << "\nDeleted Photo restored successfully!" << endl;
	}
	else if (operation->getOperationName() == deleteCategoryOp) {
		static_cast<UndoDeleteCategory*>(operation)->undo(categories, size, categoryCount);
		cout << "\nDeleted category restored successfully!" << endl;
	}
}

// Displays all the available menu options.
void displayMainMenuOptions() {
	cout << "\n-----------------\n";
	cout << "|---Main Menu---|\n";
	cout << "-----------------\n" << endl;
	cout << "0-  Exit\n"
		<< "1-  Add Photo\n"
		<< "2-  Delete Photo\n"
		<< "3-  Add Category\n"
		<< "4-  Delete Category\n"
		<< "5-  Add photo to a category\n"
		<< "6-  Remove photo from a category\n"
		<< "7-  Display category\n"
		<< "8-  Display union of categories\n"
		<< "9-  Display intersection of categories\n"
		<< "10- Display XOR of categories\n"
		<< "11- Display photos with a specific title\n"
		<< "12- Display photos taken on a specific date\n"
		<< "13- Undo\n" << endl;
}

// Main loop that takes user input and calls respective functions.
void mainMenu(
	PhotoCategory*& categories,
	int& size,
	int& categoryCount
	) {

	while (true) {
		displayMainMenuOptions();
		cout << "Select an option: ";
		int option;
		cin >> option;

		if (!cin || option < 0 || option > 13) {
			cout << "\nInvalid Option! Try again." << endl;
			continue;
		}
		// Exit
		if (option == 0) {
			cout << "\Thank You for using Photo Organizer V4!" << endl; return;
		}
		// Add Photo
		else if (option == 1) {
			string title;
			cout << "\nEnter the title of the photo: ";
			cin >> title;

			string name;
			cout << "Enter the file name: ";
			cin >> name;

			string path;
			int pathOption;
			cout << "\nDo you want to add photos from the current folder or from somewhere else?\n1- Current Folder\n2- Another folder\n" << endl;
			while (true) {
				cin >> pathOption;
				if (!cin || pathOption < 1 || pathOption > 2) {
					cout << "Invalid Option! Try again." << endl;
					continue;
				}
				else {
					if (pathOption == 1) {
						path = getCurrentDirectory();
						break;
					}
					else {
						cin.ignore();
						cout << "\nEnter photo path: ";
						getline(cin, path);
						break;
					}
				}
			}
			path = getCurrentDirectory();

			string date;
			cout << "\nEnter the date of creation (DD/MM/YY): ";
			while (true) {
				cin >> date;
				if (!cin || date.length() != 8) {
					cout << "Invalid Date! Try again." << endl;
					continue;
				}
				else {
					break;
				}
			}
			addPhoto(categories, size, categoryCount, title, name, path, date);
			cout << "\nPhoto added successfully!" << endl;
		}
		// Delete Photo
		else if (option == 2) {
			string title;
			cout << "\nEnter title of photo: ";
			cin >> title;

			string fileName;
			cout << "Enter file name: ";
			cin >> fileName;

			deletePhoto(categories, size, categoryCount, title, fileName);

		}
		// Add category
		else if (option == 3) {
			string name;
			cout << "\nEnter category name: ";
			cin >> name;

			addCategory(categories, size, categoryCount, name);
			cout << "\nCategory added successfully!" << endl;
		}
		// Remove category
		else if (option == 4) {
			string categoryToDelete;
			while (true) {
				cout << "\nCategory to be deleted: ";
				cin >> categoryToDelete;
				if (categoryToDelete != "Default") break;
				else cout << "\nInvalid category!";
			}
			deleteCategory(categories, size, categoryCount, categoryToDelete);
			cout << "\nCategory deleted successfully!" << endl;
		}
		// Add photo category
		else if (option == 5) {
			string title;
			cout << "\nEnter title of photo: ";
			cin >> title;

			string fileName;
			cout << "Enter file name: ";
			cin >> fileName;



			addPhotoCategory(categories, size, categoryCount, title, fileName);
			cout << "\nPhoto added to category successfully!" << endl;
		}
		// Remove photo category
		else if (option == 6) {
			string title;
			cout << "\nEnter title of photo: ";
			cin >> title;

			string fileName;
			cout << "Enter file name: ";
			cin >> fileName;

			deletePhotoCategory(categories, size, categoryCount, title, fileName);
			cout << "\nPhoto successfully removed from category" << endl;
		}
		else if (option == 7) displayCategory(categories, size, categoryCount);
		else if (option == 8) displayUnion(categories, size, categoryCount);
		else if (option == 9) displayIntersection(categories, size, categoryCount);
		else if (option == 10) displayXor(categories, size, categoryCount);
		else if (option == 11) displayTitleWise(categories, size, categoryCount);
		else if (option == 12) displayDateWise(categories, size, categoryCount);
		else if (option == 13) undo(categories, size, categoryCount);
	}
}
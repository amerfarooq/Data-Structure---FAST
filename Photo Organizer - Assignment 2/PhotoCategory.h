#pragma once
#include <string>

using namespace std;

struct PhotoNode;

class PhotoCategory {
	
public:
	string categoryName;
	PhotoNode* start;
	int totalPhotos;


	PhotoCategory(string = "");
	~PhotoCategory();

	// Append a PhotoNode to the PhotoCategory.
	void append(PhotoNode* const&); 
	
	// Return the name of the PhotoCategory.
	const string getName () const {
		return categoryName;
	}

	// Set the name of the PhotoCategory.
	void setName(string name) {
		this->categoryName = name;
	}

	// Removes AuxNode of this category from each of its PhotoNodes and sets head to NULL.
	void deleteCategory();

	// Only for Default category. Called after deleteCategory to remove all the PhotoNodes which
	// only belong to the Default category.
	void checkDefaultCategory();

};


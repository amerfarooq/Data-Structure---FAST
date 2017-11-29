#include "stdafx.h"
#include "MenuFunctions.h"

using namespace std;

int main() {
	// TODO: 1- Fix addPhoto 2- Try to implement current pointer 3- Remove dispaly func from PhotoCategory
	// Adding category with same name // Write destructors
	int totalCategories = 10;
	PhotoCategory* categories = new PhotoCategory[totalCategories];
	categories[0].setName("Default");
	categories[1].setName("Friends");
	categories[2].setName("Trips");
	categories[3].setName("Family");
	int currentCategoryCount = 4;
	
	mainMenu(categories, totalCategories, currentCategoryCount); 
}


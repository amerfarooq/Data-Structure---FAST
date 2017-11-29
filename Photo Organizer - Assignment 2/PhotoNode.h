#pragma once

#include <string>
using namespace std;

struct AuxNode;

struct PhotoNode {
	string title;
	string fileName;
	string filePath;
	string dateOfCreation;
	string* categories;
	int categoryCount;
	int totalCategories;
	AuxNode* start;

	PhotoNode(string title = "", string fileName = "", string filePath = "", string dateOfCreation = "");
	void copy(const PhotoNode&);
	AuxNode*& addCategoryAuxNode(const string category);
	PhotoNode*& getCategoryTarget(const string category);
	AuxNode*& getCategoryAuxNode(const string category);
	void updateAuxNode(const string category, PhotoNode* const& targetPhoto, PhotoNode* const& prevPhoto);
	PhotoNode*& deleteCategoryAuxNode(const string category);
	bool isThisCategoryInPhoto(const string category);
	int getCategoryCount() { return categoryCount; }
};
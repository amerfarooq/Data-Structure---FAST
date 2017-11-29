#include "stdafx.h"
#include "PhotoNode.h"
#include "AuxNode.h"
#include <iostream>

PhotoNode::PhotoNode(string title, string fileName, string filePath, string dateOfCreation)
{
	this->title = title;
	this->fileName = fileName;
	this->filePath = filePath;
	this->dateOfCreation = dateOfCreation;
	this->categoryCount = 0;
	this->start = nullptr;
	this->totalCategories = 10;
	this->categories = new string[totalCategories];
}

void PhotoNode::copy(const PhotoNode& copyPhoto)
{
	this->title = copyPhoto.title;	
	this->fileName = copyPhoto.fileName;
	this->filePath = copyPhoto.filePath;
	this->dateOfCreation = copyPhoto.dateOfCreation;
}

// Will be called when a new PhotoNode is appended to a PhotoCategory.
AuxNode*& PhotoNode::addCategoryAuxNode(const string category) 
{
	if (this->categoryCount == this->totalCategories) {
		this->totalCategories *= 2;
		string* newCategories = new string[totalCategories];

		for (int i = 0; i < this->categoryCount; ++i) {
			newCategories[i] = this->categories[i];
		}
		delete[] this->categories;
		this->categories = newCategories;
	}

	categories[categoryCount++] = category;
	AuxNode* newAuxNode = new AuxNode(category);
	
	if (this->start == nullptr) {
		this->start = newAuxNode;
	}
	else {
		AuxNode* iterator = start;
		while (true) {
			if (iterator->next == nullptr) {
				iterator->next = newAuxNode;
				break;
			}
			iterator = iterator->next;
		}
	}
	return newAuxNode;
}

PhotoNode*& PhotoNode::getCategoryTarget(const string category)
{
	AuxNode* iterator = start;
	while (iterator != nullptr) {
		if (iterator->category == category) 
			return iterator->target;
		else 
			iterator = iterator->next;
	}
}

AuxNode*& PhotoNode::getCategoryAuxNode(const string category) 
{
	AuxNode* iterator = start;

	while (iterator != nullptr) {
		if (iterator->category == category)
			return iterator;
		else
			iterator = iterator->next;
	}
}

void PhotoNode::updateAuxNode(const string category, PhotoNode* const& targetPhoto, PhotoNode* const& prevPhoto)
{
	AuxNode* iterator = start;

	while (iterator != nullptr) {
		if (iterator->category == category) {
			AuxNode* newAuxNode = targetPhoto->addCategoryAuxNode(category);
			newAuxNode->prev = prevPhoto;
			iterator->target = targetPhoto;
			break;
		}
		else
			iterator = iterator->next;
	}
}

PhotoNode*& PhotoNode::deleteCategoryAuxNode(const string categoryToDelete)
{
	// Removing categoryToDelete from categories string array.
	for (int i = 0; i < categoryCount; ++i) {
		if (categories[i] == categoryToDelete) { 
			if (i + 1 == categoryCount) categories[i].erase();
			else {
				for (int j = i; j < categoryCount; ++j)
					categories[j] = categories[j + 1];
			}
			--categoryCount;
		}
	}
	
	AuxNode* iterator = start;
	AuxNode* prevNode = nullptr;

	while (iterator != nullptr) {
		
		if (iterator->category == categoryToDelete) {
			
			if (prevNode == nullptr) 
				this->start = iterator->next;
			else 
				prevNode->next = iterator->next;
	
			PhotoNode* targetNode = iterator->target;
			delete iterator;
			return targetNode;
		}
		else {
			prevNode = iterator;
			iterator = iterator->next;
		}
	}
}

bool PhotoNode::isThisCategoryInPhoto(const string category)
{
	for (int i = 1; i < totalCategories; ++i) {
		if (categories[i] == category) return true;
	}
	return false;
}


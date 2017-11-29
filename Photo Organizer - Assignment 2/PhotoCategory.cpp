#include "stdafx.h"
#include "PhotoCategory.h"
#include "PhotoNode.h"
#include "AuxNode.h"
#include <windows.h>
#include <Shellapi.h>
#include <string>
#include <iostream>
#include <fstream>

PhotoCategory::PhotoCategory(string categoryName) {
	this->categoryName = categoryName;
	this->start = nullptr;
	this->totalPhotos = 0;
}

PhotoCategory::~PhotoCategory() {
	//delete start;
}

void PhotoCategory::append(PhotoNode* const& newPhoto) {
	if (this->start == nullptr) {
		this->start = newPhoto;
		newPhoto->addCategoryAuxNode(this->categoryName);
	}
	else {
		PhotoNode* iterator = this->start;
		while (iterator != nullptr) {
			
			PhotoNode* nextNode = iterator->getCategoryTarget(this->categoryName);

			if (nextNode == nullptr) {
				iterator->updateAuxNode(this->categoryName, newPhoto, iterator);
				break;
			}
			else {
				iterator = nextNode;
			}
		}
	}
	++totalPhotos;
}

void PhotoCategory::deleteCategory()
{
	PhotoNode* iterator = this->start;
	while (iterator != nullptr) {
		iterator = iterator->deleteCategoryAuxNode(this->categoryName);
	}
	this->start = nullptr;
	this->categoryName = "";
}

void PhotoCategory::checkDefaultCategory()
{
	if (this->categoryName != "Default") return;
	
	PhotoNode* iterator = this->start;
	while (iterator) {
		// If PhotoNode only belongs to one category i.e. the Default Category
		if (iterator->getCategoryCount() == 1) {
			
			AuxNode* defaultAuxNode = iterator->getCategoryAuxNode("Default");
			if (defaultAuxNode->prev == nullptr) {
				this->start = defaultAuxNode->target;
				if (defaultAuxNode->target != nullptr)
					defaultAuxNode->target->getCategoryAuxNode("Default")->prev = nullptr;
			}
			else {
				AuxNode* prevDefaultAuxNode = defaultAuxNode->prev->getCategoryAuxNode("Default");
				prevDefaultAuxNode->target = defaultAuxNode->target;
				if (defaultAuxNode->target != nullptr)
					defaultAuxNode->target->getCategoryAuxNode("Default")->prev = defaultAuxNode->prev;
			}
			PhotoNode* temp = iterator->deleteCategoryAuxNode("Default");
			delete iterator;
			iterator = temp;
		}
		else {
			iterator = iterator->getCategoryTarget("Default");
		}
	}
}

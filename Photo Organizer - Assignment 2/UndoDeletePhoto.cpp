#include "stdafx.h"
#include "UndoDeletePhoto.h"
#include <iostream>

UndoDeletePhoto::UndoDeletePhoto(PhotoNode*& savedPhoto) 
	: UndoOperation(deletePhotoOp)
{
	savedPhotoNode = new PhotoNode;
	(*savedPhotoNode).copy(*savedPhoto);
	
	this->totalCategories = savedPhoto->totalCategories;
	this->photoCategories = new string[this->totalCategories];
	this->categoryCount = 0;
	
	for (int i = 0; i < totalCategories; ++i) 
		this->photoCategories[categoryCount++] = savedPhoto->categories[i];
}

void UndoDeletePhoto::undo(PhotoCategory*& categories, int size, int categoryCount) 
{
	for (int i = 0; i < this->categoryCount; ++i) {
		for (int j = 0; j < categoryCount; ++j) {
			if (photoCategories[i] == categories[j].getName()) {
				categories[j].append(savedPhotoNode);
				break;
			}
		}
	}
}



#include "stdafx.h"
#include "UndoDeleteCategory.h"


UndoDeleteCategory::UndoDeleteCategory(const PhotoCategory& photoCategory)
	: UndoOperation(deleteCategoryOp), delPhotoCount(0), delCategoryCount(0)
{
	this->categoryName = photoCategory.categoryName;
	this->deletedPhotos = new PhotoNode[photoCategory.totalPhotos];
	this->deletedCategories = new PhotoNode*[photoCategory.totalPhotos];

	PhotoNode* iterator = photoCategory.start;
	while (iterator) {
		if (iterator->getCategoryCount() - 1 == 1) {		// Default category is not being counted
			this->deletedPhotos[delPhotoCount++].copy(*iterator);
		}
		else {
			this->deletedCategories[delCategoryCount++] = iterator;
		}
		iterator = iterator->getCategoryTarget(photoCategory.categoryName);
	}
}

void UndoDeleteCategory::undo(PhotoCategory*& categoriesArr, int& size, int& categoryCount)
{
	categoriesArr[categoryCount].setName(this->categoryName);
	
	// Appending deleted photos to the category.
	for (int i = 0; i < this->delPhotoCount; ++i) {
		categoriesArr[0].append(&deletedPhotos[i]);
		categoriesArr[categoryCount].append(&deletedPhotos[i]);
	}
	// Appending photos with deleted AuxNodes.
	for (int i = 0; i < this->delCategoryCount; ++i) {
		categoriesArr[categoryCount].append(deletedCategories[i]);
	}
	++categoryCount;
}


//UndoDeleteCategory::~UndoDeleteCategory()
//{
//}

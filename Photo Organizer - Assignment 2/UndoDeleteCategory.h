#pragma once
#include "UndoOperation.h"
#include "PhotoNode.h"
#include "PhotoCategory.h"
class UndoDeleteCategory : public UndoOperation
{
public:
	UndoDeleteCategory(const PhotoCategory&);
	void undo(PhotoCategory*&, int& size, int& categoryCount);

	string categoryName;
	PhotoNode* deletedPhotos;
	PhotoNode** deletedCategories;
	int delPhotoCount;
	int delCategoryCount;
};


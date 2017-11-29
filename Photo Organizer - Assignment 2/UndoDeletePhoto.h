#pragma once
#include "UndoOperation.h"
#include "PhotoCategory.h"
#include "AuxNode.h"

class UndoDeletePhoto : public UndoOperation 
{
public:
	UndoDeletePhoto(PhotoNode*&);
	void undo(PhotoCategory*&, int size, int categoryCount);
	
	PhotoNode* savedPhotoNode;
	string* photoCategories;
	int totalCategories;
	int categoryCount;
};


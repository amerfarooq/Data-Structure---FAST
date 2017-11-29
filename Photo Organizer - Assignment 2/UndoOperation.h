#pragma once
#include <string>

using namespace std;

enum operation {
	deletePhotoOp, deleteCategoryOp
};

class UndoOperation
{
public:
	UndoOperation(operation);
	const operation getOperationName() { return operationEnum; }

protected:
	operation operationEnum;
};


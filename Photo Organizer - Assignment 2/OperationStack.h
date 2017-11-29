#pragma once

#include "UndoDeleteCategory.h"
#include "UndoDeletePhoto.h"

struct StackNode {
	StackNode* next;
	UndoOperation* operation;

	StackNode()
		: next(nullptr),
		  operation(nullptr)
	{
	}
	StackNode(UndoOperation* op)
		: next(nullptr),
		  operation(op)
	{
	}
};

class OperationStack
{
public:
	OperationStack();
	void push(UndoOperation*);
	UndoOperation* pop();

//private:
	StackNode* top;
	StackNode* head;
};


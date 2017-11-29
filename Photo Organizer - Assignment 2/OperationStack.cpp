#include "stdafx.h"
#include "OperationStack.h"
#include <iostream>

OperationStack::OperationStack()
	: top(nullptr),
	  head(nullptr)
{
}

void OperationStack::push(UndoOperation* operation)
{
	if (top == nullptr) {
		top = new StackNode(operation);
		head = top;
	}
	else {
		top->next = new StackNode(operation);
		top = top->next;
	}
}

UndoOperation* OperationStack::pop()
{
	StackNode* iterator = this->head;
	StackNode* prev = nullptr;
	while (iterator) {
		if (iterator == top) {
			UndoOperation* returnOp = iterator->operation;

			if (prev) prev->next = nullptr;
			else head = nullptr;
	
			delete iterator;
			top = prev;
 			return returnOp;
		}
		else {
			prev = iterator;
			iterator = iterator->next;
		}
	}
	return nullptr;
}


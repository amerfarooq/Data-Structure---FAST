#include "stdafx.h"
#include "UndoOperation.h"
#include <iostream>

UndoOperation::UndoOperation(operation opName)
		: operationEnum(opName)
{
}

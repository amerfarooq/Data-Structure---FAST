#pragma once

#include "PhotoNode.h"

struct AuxNode {
	string category;
	AuxNode* next;
	PhotoNode* prev;
	PhotoNode* target;

	AuxNode(string category = "");
};

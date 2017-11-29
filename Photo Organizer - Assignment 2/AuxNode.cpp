#include "stdafx.h"
#include "AuxNode.h"

AuxNode::AuxNode(string category) {
	this->category = category;
	this->target = nullptr;
	this->next = nullptr;
	this->prev = nullptr;
}


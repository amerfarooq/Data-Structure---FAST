#pragma once

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>
#include "PhotoCategory.h"
#include "PhotoNode.h"
#include "AuxNode.h"
#include <algorithm>
#include "util.h"

using namespace std;



void displayCategoryPhotos(string category, PhotoCategory categories[], int size) {
	for (int i = 0; i < 15; ++i) {
		if (categories[i].getName() == category) {
			categories[i].display();
		}
	}
}

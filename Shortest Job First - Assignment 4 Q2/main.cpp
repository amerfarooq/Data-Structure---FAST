#include <iostream>
#include <string>
#include "BinaryHeap.h"

using namespace std;


int main() {

	BinaryHeap h;

	h.insert(121);
	h.insert(57);
	h.insert(165);
	
	h.display();

	system("pause");
}

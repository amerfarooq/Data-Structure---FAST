#pragma once

#include <iostream>
#include <string>

using namespace std;

struct HeapNode {
	string process;
	int duration;

	HeapNode() {
		process = "wtf";
		duration = -1;
	}
	void set(string pr, int time) {
		process = pr;
		duration = time;
	}
};


class BinaryHeap {

public:

	BinaryHeap() {
		//heapArray = new HeapNode[50];
		this->arraySize = 50;
	}

	void insert(/*string process, */ int duration) {
		// Inserting first element
		if (cursorPosition == -1) {
			cursorPosition = 1;
			heapArray[cursorPosition].set("", duration);
		}
		else {
			// Insert at left child of cursor
			if (heapArray[cursorPosition * 2].duration == -1) {
				heapArray[cursorPosition * 2].set("", duration);
				++arrayCounter;

				if (arrayCounter == arraySize - 1) resize();
			}
			// Insert at right child of cursor
			else if (heapArray[cursorPosition * 2 + 1].duration == -1) {
				heapArray[cursorPosition * 2 + 1].set("", duration);
				++arrayCounter;

				if (arrayCounter == arraySize - 1) resize();
			}
			else {
				if (level == 0) {
					level++;
					cursorPosition *= 2;
				}
				else {
					// Inserting at same depth so cursor incremented
					if (level < level * 2) {
						cursorPosition++;
						levelCounter++;
					}
					// Going one level deeper
					else {
						// Returns index of first node at the same depth
						cursorPosition = 2 * (cursorPosition - (2 * level - 1));
						level *= 2;
						levelCounter = 1;
					}
				}
			}
		}
	}

	void resize() {
		//HeapNode* temp = heapArray;
		//heapArray = new HeapNode[arraySize * 2];

		//for (int i = 0; i < arraySize; ++i)
		//	heapArray[i] = temp[i];
	}

	void display() const {
		for (int i = 1; i <= arraySize; ++i) {
			cout << heapArray[i].duration << " ";
		}
	}

	~BinaryHeap() {
		//delete[] heapArray;
	}

private:
	int cursorPosition = -1;
	int level = 0;
	int levelCounter = 1;
	int arraySize;
	int arrayCounter = 0;
	//HeapNode* heapArray;
	HeapNode heapArray[50];
};
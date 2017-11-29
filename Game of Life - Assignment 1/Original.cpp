#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
using namespace  std;

struct Cell {
	int x, y;
	bool isAlive;

	Cell() {
		x = y = -1;
		isAlive = false;
	}
};

ostream& operator<<(ostream& os, const Cell& cell) {
	os << "(" << cell.x << "," << cell.y << ")" << endl;
	return os;
}

void displayBoard(int** const board, const int rows, const int cols);
/*
If the cell is alive, adds its neighbours to the cellArray if they aren't already in it and counts the number of dead and alive neigbours of the cell to evaluate
whether the it will propogate to the next generation. If the cell is dead, only counts the neighbours and performs the evaluation. If the cell will live on, returns
true. If the cell will die, returns false.
*/
bool updateCell(Cell* cellArray, int cellNumber, int& cellCount, bool isAlive, int** const board, int rows, int cols);

int main() {

	cout << "Welcome to the Game of Life!\n" << endl;

	int rows, cols;
	cout << "Enter number of rows: ";
	cin >> rows;
	cout << "Enter number of columns: ";
	cin >> cols;

	// Form the board.
	int** const board = new int*[rows];
	for (int i = 0; i < rows; ++i) {
		board[i] = new int[cols];
		for (int j = 0; j < cols; ++j)
			board[i][j] = -1;
	}

	// Read contents from file.
	string fileName;
	ifstream inFile;
	while (true) {

		cout << "Enter file name: ";
		cin >> fileName;
		inFile.open(fileName.c_str());

		if (!inFile) cerr << "\nERROR: Could not locate file!\n" << endl;
		else break;
	}

	int totalGenerations, totalLiveCells;
	inFile >> totalGenerations >> totalLiveCells;

	// Since each cell has 8 neighbours, for N cells, we need to check a maximum of 8N board locations.
	Cell* cellArray = new Cell[totalLiveCells * 9];
	int cellRow, cellCol, cellCount = 0;

	// Add the initial live cells to cellArray and the board.
	while (inFile >> cellRow >> cellCol) {
		board[cellRow][cellCol] = cellCount;
		cellArray[cellCount].isAlive = true;
		cellArray[cellCount].x = cellRow;
		cellArray[cellCount].y = cellCol;
		++cellCount;
	}

	for (int i = 0; i < totalGenerations; ++i) {

		cout << "Generation: " << i + 1 << endl;
		displayBoard(board, rows, cols);
		cout << endl;

		// nextGenCellArray stores index number of cells from cellArray that will propogate to the next generation.
		int* nextGenCellsArray = new int[totalLiveCells * 9];
		int noOfNextGenCells = 0;
		for (int j = 0; j < totalLiveCells * 9 && cellArray[j].x != -1; ++j) {
			// If the cell propogates to the next generation, add it to nextGenCellArray.
			if (cellArray[j].isAlive) {
				if (updateCell(cellArray, j, cellCount, true, board, rows, cols)) {
					nextGenCellsArray[noOfNextGenCells] = j;
					++noOfNextGenCells;
				}
			}
			else {
				if (updateCell(cellArray, j, cellCount, false, board, rows, cols)) {
					nextGenCellsArray[noOfNextGenCells] = j;
					++noOfNextGenCells;
				}
			}
		}
		// Update cellCount and totalLiveCells for the next generation.
		int oldCellCount = cellCount;
		totalLiveCells = cellCount = noOfNextGenCells;

		// Save current cellArray into tempArray and initialize cellArray again to store the
		// initial alive cells of the next generation.
		Cell* tempArray = cellArray;
		cellArray = new Cell[totalLiveCells * 8 + totalLiveCells];
		for (int i = 0; i < cellCount; ++i) {
			cellArray[i] = tempArray[nextGenCellsArray[i]];
			cellArray[i].isAlive = true;
			board[cellArray[i].x][cellArray[i].y] = i;
		}

		for (int i = 0; i < oldCellCount; ++i) {
			// If cell at index i in tempArray is not propogating to next generation, kill it.
			if (find(nextGenCellsArray, nextGenCellsArray + noOfNextGenCells, i) == nextGenCellsArray + noOfNextGenCells) {
				// Go to its corresponding board location and set its integer value to -1 and
				// set its status to dead.
				board[tempArray[i].x][tempArray[i].y] = -1;
				tempArray[i].isAlive = false;
			}
		}
		delete[] tempArray;
		delete[] nextGenCellsArray;
		this_thread::sleep_for(chrono::microseconds(200000));
		system("CLS");
	}

	delete[] cellArray;
	for (int i = 0; i < rows; ++i) delete[] board[i];
	delete[] board;
}

void displayBoard(int** const board, const int rows, const int cols) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (board[i][j] != -1) cout << "X";
			else cout << '.';
		}
		cout << endl;
	}
}

bool updateCell(Cell* cellArray, int cellNumber, int& cellCount, bool isAlive, int** const board, int rows, int cols) {

	Cell& currCell = cellArray[cellNumber];
	int x = currCell.x;
	int y = currCell.y;

	int deadNeighbours = 0;
	int liveNeighbours = 0;

	if (y - 1 >= 0) {
		if (board[x][y - 1] != -1 && cellArray[board[x][y - 1]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x][y - 1] == -1) {
				board[x][y - 1] = cellCount;
				cellArray[cellCount].x = x;
				cellArray[cellCount].y = y - 1;
				++cellCount;
			}
		}
	}

	if (y - 1 >= 0 && x + 1 < rows) {
		if (board[x + 1][y - 1] != -1 && cellArray[board[x + 1][y - 1]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x + 1][y - 1] == -1) {
				board[x + 1][y - 1] = cellCount;
				cellArray[cellCount].x = x + 1;
				cellArray[cellCount].y = y - 1;
				++cellCount;
			}
		}
	}

	if (x + 1 < rows) {
		if (board[x + 1][y] != -1 && cellArray[board[x + 1][y]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x + 1][y] == -1) {
				board[x + 1][y] = cellCount;
				cellArray[cellCount].x = x + 1;
				cellArray[cellCount].y = y;
				++cellCount;
			}
		}
	}

	if (y + 1 < cols && x + 1 < rows) {
		if (board[x + 1][y + 1] != -1 && cellArray[board[x + 1][y + 1]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x + 1][y + 1] == -1) {
				board[x + 1][y + 1] = cellCount;
				cellArray[cellCount].x = x + 1;
				cellArray[cellCount].y = y + 1;
				++cellCount;
			}
		}
	}

	if (y + 1 < cols) {
		if (board[x][y + 1] != -1 && cellArray[board[x][y + 1]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x][y + 1] == -1) {
				board[x][y + 1] = cellCount;
				cellArray[cellCount].x = x;
				cellArray[cellCount].y = y + 1;
				++cellCount;
			}
		}
	}

	if (y + 1 < cols && x - 1 >= 0) {
		if (board[x - 1][y + 1] != -1 && cellArray[board[x - 1][y + 1]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x - 1][y + 1] == -1) {
				board[x - 1][y + 1] = cellCount;
				cellArray[cellCount].x = x - 1;
				cellArray[cellCount].y = y + 1;
				++cellCount;
			}
		}
	}

	if (x - 1 >= 0) {
		if (board[x - 1][y] != -1 && cellArray[board[x - 1][y]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x - 1][y] == -1) {
				board[x - 1][y] = cellCount;
				cellArray[cellCount].x = x - 1;
				cellArray[cellCount].y = y;
				++cellCount;
			}
		}
	}

	if (y - 1 >= 0 && x - 1 >= 0) {
		if (board[x - 1][y - 1] != -1 && cellArray[board[x - 1][y - 1]].isAlive) {
			++liveNeighbours;
		}
		else {
			++deadNeighbours;
			if (isAlive && board[x - 1][y - 1] == -1) {
				board[x - 1][y - 1] = cellCount;
				cellArray[cellCount].x = x - 1;
				cellArray[cellCount].y = y - 1;
				++cellCount;
			}
		}
	}

	if (isAlive) {
		if (liveNeighbours >= 2 && liveNeighbours <= 3) return true;
		else return false;
	}
	else {
		if (liveNeighbours == 3) return true;
		else return false;
	}
}
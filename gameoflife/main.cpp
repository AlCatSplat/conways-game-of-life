/*
* Conway's game of life in C++. Created by Alex Kibret, Raine Wong, and Andy Xiao.
*/
#include <iostream>
#include <fstream>
#include <string>

void readFromFile(int cellArray, int xSize, int ySize);

int main() {

	const int X_SIZE = 30;
	const int Y_SIZE = 30;

	int cells[X_SIZE][Y_SIZE];

	readFromFile(cells[X_SIZE][Y_SIZE], X_SIZE, Y_SIZE);

	return 0;
}

// TODO: Implement ability to read game state from CSV file
void readFromFile(int cellArray, int xSize, int ySize) {

	std::ifstream CellDataFile("cells.csv");
	std::string line;
	bool firstRow = true;

	for (int i = 0; i < xSize; i++) {
		//Work in progress
	}

	while (std::getline(CellDataFile, line)) {
		if (firstRow) {
			firstRow = false;
			continue;
		}
	}

}
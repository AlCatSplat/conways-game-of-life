/*
* Conway's game of life in C++. Created by Alex Kibret, Raine Wong, and Andy Xiao.
*/
#include <iostream>
#include <fstream>

void readFromFile();

int main() {

	std::cout << "Hello world.";

	int cells[30][30];

	return 0;
}

//TODO: Implement ability to read game state from CSV file
void readFromFile() {

	std::ifstream CellDataFile("cells.csv");

}
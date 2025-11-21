/*
* Conway's game of life in C++. Created by Alex Kibret, Raine Wong, and Andy Xiao.
* Version 2025.11.20
*/
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
using namespace std;

const int SIZE = 30;
const int ALIVE = 1;
const int DEAD = 0;
const int TOTAL_VARIABLES = 3;
const int GAMES_MAX = 200;
const int YEARS_MAX = 30;

void gameUI(int cellArray[SIZE][SIZE]);
void readFromFile(int cellArray[SIZE][SIZE], std::string cellFile);
double randomDouble();
void fillArray(int array[SIZE][SIZE], double odds);
int countSurroundingAlive(int arr[SIZE][SIZE], int row, int col);
int getNumAfterYear(int count, int currentState);
void simYear(int arr[SIZE][SIZE], int& year);
void summarizeResults(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE], int year);
void simYearSetUp(int arr[SIZE][SIZE]);
int* getHighScoreData();
int (*getHighScoreArr())[SIZE];
int (*getRecordData())[TOTAL_VARIABLES];
int& getTotalGamesCompleted();//might not need this prototype
void cleanupPointers();
void randomizeMultipleGames(int times, double odds);
void summaryReport();
void cleanupGameData();
std::string askForFileName();
void displayGameState(int cellArray[SIZE][SIZE], std::string cellFile);
double askForOdds();
int askForTimes();
void getBestBoardValue();

int main() {

	int cells[SIZE][SIZE];

	gameUI(cells);

	int (*arrHighScore)[SIZE] = getHighScoreArr();//intialize pointers for the array that results with the most alive (best array)
	int (*arrRecord)[3] = getRecordData();//record of all games data
	int* highScoreData = getHighScoreData();//data of the best array

	//By using these lines, you can access the data from these arrays. Either pass this into ur function or paste this into the 
	//function itself-test 2

	//Useful functions: simYearSetUp(arr) will run one instance of the game of life and returns void
	//getTotalGamesCompleted will give the totalgames done, starting from 1. You can use this to know how may
	//rows to use from the array, but be aware to substract 1 as indexing starts at 0
	//play random games with randomizeMultipleGames	

	//free memory
	cleanupGameData();
	return 0;
}

// Reads game state from CSV file
void readFromFile(int cellArray[SIZE][SIZE], std::string cellFile) {

	std::ifstream cellDataFile(cellFile);

	if (!cellDataFile.is_open()) {
		std::cout << "Error: File not found." << std::endl;
		return;
	}
	else {
		std::cout << "File read successfully." << std::endl;
	}

	std::string line;
	int row = 0;

	std::getline(cellDataFile, line);

	while (std::getline(cellDataFile, line)) {
		std::stringstream ss(line);
		std::string cell;
		int column = 0;

		std::getline(ss, cell, ',');

		while (std::getline(ss, cell, ',')) {
			if (column < SIZE) {
				cellArray[row][column] = std::stoi(cell);
				column++;
			}
		}
		row++;
		if (row >= SIZE) {
			break;
		}
	}
	cellDataFile.close();
}

//saves total games - Raine
int& getTotalGamesCompleted() {
	static int totalGames = 0;
	return totalGames;
}

void simYearSetUp(int arr[SIZE][SIZE]) {
	int year = 0;
	static int changingArr[SIZE][SIZE];

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			changingArr[i][j] = arr[i][j];
		}
	}

	simYear(changingArr, year);
	summarizeResults(changingArr, arr, year);

	//increment games
	getTotalGamesCompleted()++;
	return;
}

//pointers for useful arrays - Raine
int* getHighScoreData() {
	static int* highScoreData = new int[TOTAL_VARIABLES] {};
	return highScoreData;
}

int (*getHighScoreArr())[SIZE] {
	static int (*arrHighScore)[SIZE] = new int[SIZE][SIZE]{};
	return arrHighScore;
}

int (*getRecordData())[TOTAL_VARIABLES] {
	static int (*arrRecord)[TOTAL_VARIABLES] = new int[GAMES_MAX][TOTAL_VARIABLES]{};
	return arrRecord;
}

//simulate the game - Raine
void simYear(int arr[SIZE][SIZE], int& year) {
	bool noChange = true;
	bool allDead = true;

	int tempArr[SIZE][SIZE] = { 0 };

	//instructions say to start at year 0
	cout << "Current year: " << year << endl;

	//sim one year
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			int aliveCount = countSurroundingAlive(arr, i, j);
			tempArr[i][j] = getNumAfterYear(aliveCount, arr[i][j]);

			if (arr[i][j] != tempArr[i][j]) {
				noChange = false;
			}

			if (tempArr[i][j] == ALIVE) {
				allDead = false;
			}
		}
	}

	//copy temp arr and cout
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			arr[i][j] = tempArr[i][j];
			cout << arr[i][j];
		}
		cout << endl;
	}

	//one year has passed
	year++;

	//check if game has ended
	if (noChange || allDead || year >= YEARS_MAX) {
		return;
	}

	//loop
	simYear(arr, year);
}

//function to count the number of 1's in the surrounding cells - Raine
int countSurroundingAlive(int arr[SIZE][SIZE], int row, int col) {
	int count = 0;

	//check all 8 surrounding positions
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {

			//skip the center cell itself
			if (i == 0 && j == 0) {
				continue;
			}

			int newRow = row + i;
			int newCol = col + j;

			//check if the index is within bounds, if it is, check for '1'
			if (newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE) {
				if (arr[newRow][newCol] == ALIVE) {
					count++;
				}
			}
		}
	}
	return count;
}

//determine dead/alive - Raine
int getNumAfterYear(int count, int currentState) {
	if (count == 3) {
		return ALIVE;
	}
	else if (count == 2 && currentState == ALIVE) {
		return ALIVE;
	}
	else {
		return DEAD;
	}
}

//saves data to arrays and determines best array - Raine
void summarizeResults(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE], int year) {
	static int numAliveRecord = -1;
	static int gamesPlayed = 0;
	const int NUM_TOTAL = SIZE * SIZE;
	int numAlive = 0;

	int (*arrHighScore)[SIZE] = getHighScoreArr();
	int (*arrRecord)[TOTAL_VARIABLES] = getRecordData();
	int* highScoreData = getHighScoreData();

	//count alive
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (arr[i][j] == ALIVE) {
				numAlive++;
			}
		}
	}

	//count dead
	int numDead = NUM_TOTAL - numAlive;

	//save if there is more alive
	if (numAlive > numAliveRecord) {
		numAliveRecord = numAlive;
		highScoreData[0] = year;
		highScoreData[1] = numAlive;
		highScoreData[2] = numDead;

		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				arrHighScore[i][j] = arrOriginal[i][j];
			}
		}
	}

	//failsafe if data overflows
	if (gamesPlayed < GAMES_MAX) {
		//insert into array
		arrRecord[gamesPlayed][0] = year;
		arrRecord[gamesPlayed][1] = numAlive;
		arrRecord[gamesPlayed][2] = numDead;
		gamesPlayed++;
		cout << "Years elapsed: " << year << " Alive cells: " << numAlive
			<< " Dead cells: " << numDead;
		return;
	}
	else {
		cout << "Error! No more space!" << endl;
		return;
	}
}

//display all games' data - Raine
void summaryReport() {
	int (*arrRecord)[3] = getRecordData();
	int rows = getTotalGamesCompleted();

	string text[TOTAL_VARIABLES] = { "Years elapsed", "Alive cells", "Dead cells" };

	// Throws error if there is no record of games played - Alex
	if (rows == 0) {
		std::cout << "Error: No games played." << std::endl;
	}

	for (int i = 0; i < rows; i++) {
		cout << "Game " << (i + 1) << ": " << endl;
		for (int j = 0; j < TOTAL_VARIABLES; j++) {
			cout << text[j] << ": " << arrRecord[i][j] << endl;
		}
		cout << endl;
	}
}

//play multiple random games - Raine
void randomizeMultipleGames(int times, double odds) {
	static int ranArr[SIZE][SIZE] = { 0 };

	for (int i = 0; i < times; i++) {
		fillArray(ranArr, odds);
		simYearSetUp(ranArr);
	}
}

//fill the array with random values based on inputted odds - Raine
void fillArray(int array[SIZE][SIZE], double odds) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

			if (randomDouble() < odds) {
				array[i][j] = ALIVE;
			}
			else {
				array[i][j] = DEAD;
			}
		}
	}
}

//random num gen - Raine
double randomDouble() {
	static random_device rd;
	static default_random_engine gen(rd());
	static uniform_real_distribution<double> dis(0.0, 1.0);
	return dis(gen);
}

//free memory - Raine
void cleanupPointers() {

	int* newArr = getHighScoreData();
	delete[] newArr;
	newArr = nullptr;//techinically this does nothing

	int (*newArr2)[SIZE] = getHighScoreArr();
	delete[] newArr2;
	newArr2 = nullptr;

	int (*newArr3)[TOTAL_VARIABLES] = getRecordData();
	delete[] newArr3;
	newArr3 = nullptr;

	return;
}

void gameUI(int cellArray[SIZE][SIZE]) {

	std::string cellFile = "cells.csv";

	int userSelection = 0;

	const char* menuOptions = R"(
 -------------------------------------------------------------------------------
 Conway's Game of Life				by Raine, Alex, and Andy
 -------------------------------------------------------------------------------
 
 Welcome! What would you like to do?
 
 (1) Display the initial game state
 (2) Load a new initial state from a CSV file
 (3) Run the game with the current initial state
 (4) Give a summary of all games previously played
 (5) Run the game with a random state
 (6) Display the best initial board values
 (7) Exit the program
 )";

	std::cout << menuOptions << std::endl << "> ";
	std::cin >> userSelection;

	//Input validation
	while (!cin || userSelection > 7 || userSelection < 1 || cin.peek() != '\n') {
		std::cout << "Invalid input. Please enter an integer from 1-7." << std::endl << "> ";
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}

	switch (userSelection) {
	case 1:
		displayGameState(cellArray, cellFile);
		break;
	case 2:
		readFromFile(cellArray, askForFileName());
		break;
	case 3:
		simYearSetUp(cellArray);
		break;
	case 4:
		summaryReport();
		break;
	case 5:
		randomizeMultipleGames(askForTimes(), askForOdds());
		break;
	case 6:
		getBestBoardValue();
		break;
	case 7:
		std::cout << "Farewell!";
		break;
	}
}

void cleanupGameData() {

}

// Asks user for the cell file - Alex
std::string askForFileName() {
	std::string userInput;
	std::cout << "Enter file name: ";
	std::cin >> userInput;
	return userInput;
}

// Asks user for the odds they want for the randomized game - Alex
double askForOdds() {
	double userInput;
	std::cout << "Please input preferred odds: ";
	std::cin >> userInput;
	std::cout << std::endl;
	return userInput;
}

// Ask user for how many times they want to run the randomized game - Alex
int askForTimes() {
	int userInput;
	std::cout << "How many times would you like to run the game? ";
	std::cin >> userInput;
	std::cout << std::endl;
	return userInput;
}

// Outputs the current cell array - Alex
void displayGameState(int cellArray[SIZE][SIZE], std::string cellFile) {
	readFromFile(cellArray, cellFile);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			std::cout << cellArray[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void getBestBoardValue() {
	int (*arrHighScore)[SIZE] = getHighScoreArr();
	int* highScoreData = getHighScoreData();
}
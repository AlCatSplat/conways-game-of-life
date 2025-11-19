/*
* Conway's game of life in C++. Created by Alex Kibret, Raine Wong, and Andy Xiao.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
using namespace std;

const int SIZE = 30;

void readFromFile(int cellArray, int xSize, int ySize);
double randomDouble();
void fillArray(int array[30][30], double odds);
int countSurroundingOnes(int arr[SIZE][SIZE], int row, int col);
int getNumAfterYear(int count, int currentState);
void simYear(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE]);
void summerizeResults(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE], int year);


int main() {

	const int X_SIZE = 30;
	const int Y_SIZE = 30;

	std::cout << "Hello!";

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

//function to count the number of 1's in the surrounding cells - Raine
int countSurroundingOnes(int arr[SIZE][SIZE], int row, int col) {
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
                if (arr[newRow][newCol] == 1) {
                    count++;
                }
            }
        }
    }
    return count;
}

//simulate game (og arr is there to keep important info, simply input the same array twice into here) - Raine
void simYear(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE]) {
    static int year = 0;
    bool noChange = true;
    bool allDead = true;
    //so updating values does not interfere
    int tempArr[SIZE][SIZE] = { 0 };

    //instructions say to start at year 0
    cout << "Current year: " << year << endl;

    //sim one year
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int onesCount = countSurroundingOnes(arr, i, j);
            tempArr[i][j] = getNumAfterYear(onesCount, arr[i][j]);

            if (arr[i][j] != tempArr[i][j]) {
                noChange = false;
            }

            if (tempArr[i][j] == 1) {
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
    if (noChange || allDead || year >= 30) {
        cout << "Simulation ended at year " << year << endl;
        //record results
        summerizeResults(arr, arrOriginal, year);
        return; //Probably go to cout results
    }

    //loop
    simYear(arr, arrOriginal);
}

//determine dead/alive - Raine
int getNumAfterYear(int count, int currentState) {
    if (count == 3) {
        return 1;
    }
    else if (count == 2 && currentState == 1) {
        return 1;
    }
    else {
        return 0;
    }
}

//Very messy and inefficient, but I think? this works. Alot of pointers so yall can call these arrays if you need to
//idk if 1, 2, and 3 would be magic nums - Raine
void summerizeResults(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE], int year) {
    static int tempSize = 1;
    static int numAliveRecord = -1;
    static int gamesPlayed = 0;
    static const int NUM_TOTAL = SIZE * SIZE;
    static int (*arrBest)[SIZE] = new int[SIZE][SIZE]();
    static int (*arrRecord)[3] = new int[tempSize][3]();
    static int* bestGameData = new int[3];

    int numAlive = 0;

    if (gamesPlayed > 0) {
        int (*newArr)[3] = new int[tempSize + 1][3]();
        for (int i = 0; i < tempSize; i++) {
            for (int j = 0; j < 3; j++) {
                newArr[i][j] = arrRecord[i][j];
            }
        }
        delete[] arrRecord;
        arrRecord = newArr;
        tempSize++;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (arr[i][j] == 1) {
                numAlive++;
            }
        }
    }

    int numDead = NUM_TOTAL - numAlive;

    if (numAlive > numAliveRecord) {
        numAliveRecord = numAlive;
        bestGameData[0] = year;
        bestGameData[1] = numAlive;
        bestGameData[2] = numDead;


        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                arrBest[i][j] = arrOriginal[i][j];
            }
        }
    }
    arrRecord[gamesPlayed][0] = year;
    arrRecord[gamesPlayed][1] = numAlive;
    arrRecord[gamesPlayed][2] = numDead;

    gamesPlayed++;
}

//fill the array with random values based on inputted odds - Raine
void fillArray(int array[SIZE][SIZE], double odds) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (randomDouble() < odds) {
                array[i][j] = 1;
            }
            else {
                array[i][j] = 0;
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
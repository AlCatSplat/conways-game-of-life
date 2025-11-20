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
const int ALIVE = 1;
const int DEAD = 0;
const int TOTAL_VARIABLES = 3;
const int GAMES_MAX = 200;
const int YEARS_MAX = 30;

void readFromFile(int cellArray, int xSize, int ySize);
double randomDouble();
void fillArray(int array[SIZE][SIZE], double odds);
int countSurroundingOnes(int arr[SIZE][SIZE], int row, int col);
int getNumAfterYear(int count, int currentState);
void simYear(int arr[SIZE][SIZE], int& year);
void summerizeResults(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE], int year);
void simYearSetUp(int arr[SIZE][SIZE]);
int* getGameBestData();
int (*getBestData())[SIZE];
int (*getRecordData())[3];
int& getTotalGamesCompleted();//might not need this prototype
void cleanupPointers();

int main() {

	const int X_SIZE = 30;
	const int Y_SIZE = 30;

    int (*arrBest)[SIZE] = getBestData();//intialize pointers for the array that results with the most alive (best array)
    int (*arrRecord)[3] = getRecordData();//record of all games data
    int* bestGameData = getGameBestData();//data of the best array
    
    //By using these lines, you can access the data from these arrays. Either pass this into ur function or paste this into the 
    //function itself

    //Useful functions: simYearSetUp(arr) will run one instance of the game of life and returns void
    //getTotalGamesCompleted will give the totalgames done, starting from 1. You can use this to know how may
    //rows to use from the array, but be aware to substract 1 as indexing starts at 0

	std::cout << "Hello!";

	int cells[X_SIZE][Y_SIZE];

	readFromFile(cells[X_SIZE][Y_SIZE], X_SIZE, Y_SIZE);

    //free memory
    void cleanupGameData();
	return 0;
}

// TODO: Implement ability to read game state from CSV file
void readFromFile(int cellArray, int xSize, int ySize) {

	std::ifstream cellDataFile;
	std::string line;
	bool firstRow = true;

	cellDataFile.open("cells.csv");

	for (int i = 0; i < SIZE; i++) {
    	for (int j = 0; j < SIZE; j++) {
        	cellDataFile >> cellArray[i][j];
    	}
	}

	while (std::getline(CellDataFile, line)) {
		if (firstRow) {
			firstRow = false;
			continue;
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
    summerizeResults(changingArr, arr, year);

    //increment games
    getTotalGamesCompleted()++;
    return;
}

//pointers for useful arrays - Raine
int* getGameBestData() {
    static int* bestGameData = new int[TOTAL_VARIABLES] {};
    return bestGameData;
}

int (*getBestData())[SIZE] {
    static int (*arrBest)[SIZE] = new int[SIZE][SIZE]{};
    return arrBest;
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
            int onesCount = countSurroundingOnes(arr, i, j);
            tempArr[i][j] = getNumAfterYear(onesCount, arr[i][j]);

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
void summerizeResults(int arr[SIZE][SIZE], int arrOriginal[SIZE][SIZE], int year) {
    static int numAliveRecord = -1;
    static int gamesPlayed = 0;
    const int NUM_TOTAL = SIZE * SIZE;
    int numAlive = 0;

    int (*arrBest)[SIZE] = getBestData();
    int (*arrRecord)[TOTAL_VARIABLES] = getRecordData();
    int *bestGameData = getGameBestData();

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
        bestGameData[0] = year;
        bestGameData[1] = numAlive;
        bestGameData[2] = numDead;

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                arrBest[i][j] = arrOriginal[i][j];
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
        cout << "Years elasped: " << year << " Alive cells: " << numAlive 
             << " Dead cells: " << numDead;
        return;
    }
    else {
        cout << "Error! No more space!" << endl;
        return;
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
    
    int* newArr = getGameBestData();
    delete[] newArr;
    newArr = nullptr;//techinically this does nothing
    
    int (*newArr2)[SIZE] = getBestData();
    delete[] newArr2;
    newArr2 = nullptr;
    
    int (*newArr3)[TOTAL_VARIABLES] = getRecordData();
    delete[] newArr3;
    newArr3 = nullptr;

    return;
}

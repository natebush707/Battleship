/****************************************************************
File: Project 2 - Battleship
Description : Most fun game of Battleship of all time, probably.

Author : Kevin Clark & Nathan Bush
Class : CS 110
Date : 11 / 1 / 2020

We hereby certify that this program is entirely our own work.
**testing change for github commit**
Note: Want to update project to have multiple .cpp and header files for readability
*****************************************************************/

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cctype>
#include <iomanip>

using namespace std;

// FUNCTION: generates the initial game boards. One for the user, one to hold the secret ship locations
// PARAMETERS: const int for board size, 2D char array for the game key, 2D char array for the user game board
template<const int rows, const int cols>
void GenBoard(const int BOARD_SIZE, char(&gameKey)[rows][cols], char(&userBoard)[rows][cols])
{
	// initialize row/col variables for labels
	char rowValue = 'A';
	char colValue = '0';

	// ensure empty space in upper left hand corner
	gameKey[0][0] = ' ';
	userBoard[0][0] = ' ';

	// build row/column labels
	for (int i = 1; i < BOARD_SIZE; i++) {
		gameKey[i][0] = rowValue;
		userBoard[i][0] = rowValue;
		gameKey[0][i] = colValue;
		userBoard[0][i] = colValue;
		rowValue++;
		colValue++;
	}

	// populate blank ocean spaces
	for (int i = 1; i < BOARD_SIZE; i++)
	{
		for (int k = 1; k < BOARD_SIZE; k++)
		{
			gameKey[i][k] = '~';
			userBoard[i][k] = '~';
		}
	}

}

// FUNCTION: displays current game board to the user
// PARAMETERS: const int for board size, 2D char array for the user game board, ints representing score and high score
template<const int rows, const int cols>
void PrintBoard(const int BOARD_SIZE, const char(&userBoard)[rows][cols], int currentScore, int highScore)
{
	// display scores
	cout << endl << "Current Score: " << currentScore << "             " << "High Score: " << highScore << endl << endl;
	
	// output array
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		cout << "          ";
		for (int k = 0; k < BOARD_SIZE; k++)
		{
			cout << userBoard[i][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// FUNCTION: displays key for debugging purposes
// PARAMETERS: int for board size, 2D char array representing game key
template<const int rows, const int cols>
void PrintKey(const int BOARD_SIZE, const char(&gameKey)[rows][cols])
{
	cout << endl;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int k = 0; k < BOARD_SIZE; k++)
		{
			cout << gameKey[i][k] << " ";
		}
		cout << endl;
	}
}

// FUNCTION: checks if a ship is present at a specific coordinate on the key
// PARAMETERS: integers for row/col and 2D array representing game key
// RETURNS: TRUE if spot is empty ocean, FALSE if spot is occupied by ship
template<const int rows, const int cols>
bool CheckSpot(int row, int col, const char(&gameKey)[rows][cols])
{
	return(gameKey[row][col] == '~');
}

// FUNCTION: gets user input and return it, enter debug mode if user enters 'debug' or 'DEBUG'
// PARAMETERS: ints representing number of turns taken and game length, bool representing debug mode status
string UserInput(int turnsLeft, const int GAME_LENGTH, bool& debugMode)
{
	// get input for next shot coordinates
	string userCoord;
	cout << "You have " << GAME_LENGTH - turnsLeft << " rounds of ammunition remaining." << endl << "Input a coordinate (A0): ";
	cin >> userCoord;

	// check to see if debug mode should be activated
	if (userCoord == "debug" || userCoord == "DEBUG")
	{
		debugMode = !debugMode;
		if (debugMode)
			cout << endl << "Debug Mode Activated" << endl;
		else
			cout << endl << "Debug Mode Deactivated" << endl;
	}

	// input validation for ('A'-'J')('0'-'9') and coordinate length
	while (!cin || userCoord[0] < 'A' || userCoord[0] > 'J' || userCoord[1] < '0' || userCoord[1] > '9' || userCoord.size() != 2)
	{
		cin.clear();
		cin.ignore(100, '\n');
		cout << "Invalid coordinates, please reenter: ";
		cin >> userCoord;
	}

	return userCoord;
}

// FUNCTION: converts alpha row value to corresponding int for row indexing
// PARAMETERS: character for alpha row value
// RETURNS: converted int row value
int ConvertAlpha(char c)
{
	map<char, int> charToInt = { {'A', 1}, {'B', 2}, {'C', 3}, {'D', 4}, {'E', 5}, 
								 {'F', 6}, {'G', 7}, {'H', 8}, {'I', 9}, {'J', 10} };
	return charToInt[c];
}

// FUNCTION: calculates shot results, updates game board, and reports to user
// PARAMETERS: string for user's selected shot location, int for size of game board, 2D char array for the game key,
//             2D char array for the user board, ints representing score and high score
template<const int rows, const int cols>
void ShotResult(string shotLoc, const int BOARD_SIZE, char(&gameKey)[rows][cols], char(&userBoard)[rows][cols], int& currentScore, int& highScore)
{
	// convert input string to separate ints for indexing the matrix
	int rowIndex = ConvertAlpha(shotLoc[0]);
	int colIndex = (shotLoc[1] - '0') + 1;

	// store matrix value at shot coordinates
	char targetStatus = gameKey[rowIndex][colIndex];

	// update game board(s) based on result, output result to user
	cout << endl << endl << "              Firing on " << shotLoc << "..." << endl;

	if (targetStatus == '~')
	{
		gameKey[rowIndex][colIndex] = 'O';
		userBoard[rowIndex][colIndex] = 'O';
		PrintBoard(BOARD_SIZE, userBoard, currentScore, highScore);
		cout << "     SPLASH! You missed your target." << endl << endl;
	}
	else if (targetStatus == 'O')
	{
		PrintBoard(BOARD_SIZE, userBoard, currentScore, highScore);
		cout << "   Umm... you already missed there once," << endl << "did you think the result would be different?" << endl << endl;
	}
	else if (targetStatus == 'X')
	{
		PrintBoard(BOARD_SIZE, userBoard, currentScore, highScore);
		cout << "You already hit the enemy at those coordinates..." << endl << "           there's nothing left." << endl << endl;
	}
	else
	{
		currentScore += 25;
		gameKey[rowIndex][colIndex] = 'X';
		userBoard[rowIndex][colIndex] = 'X';
		PrintBoard(BOARD_SIZE, userBoard, currentScore, highScore);
		cout << "             BOOM! Direct hit!" << endl << endl;
	}
}

// FUNCTION: checks to see if the game has been won
// PARAMETERS: int representing board size, 2D char array for the game key, 2D char array for the user board
// RETURNS: boolean representing victory condition. true == win, false == not win
template<const int rows, const int cols>
bool VictoryCheck(const int BOARD_SIZE, const char(&gameKey)[rows][cols], const char(&userBoard)[rows][cols])
{
	bool victory;

	// check to see if game key matches user board
	for (int i = 1; i < BOARD_SIZE; i++)
	{
		for (int k = 1; k < BOARD_SIZE; k++)
		{
			victory = gameKey[i][k] == userBoard[i][k];
			if (!victory)
				return victory;
		}
	}
	return victory;
}

// FUNCTION: checks to see if another ship is in the way of ship placement, after verifying bounds
// PARAMETERS: ints for row and column values, ints for the direction and size of ship, 2D char array for the game key
// RETURNS: TRUE if the ship fits, FALSE if another ship is in the way
template<const int rows, const int cols>
bool ShipOverlap(int rowVal, int colVal, int shipDirection, int shipSize, char(&gameKey)[rows][cols])
{
	bool roomForShip = true;

	// 0: UP, 1: DOWN, 2: LEFT, 3: RIGHT
	// Check if there's a ship in the way based on starting coordinate and direction
	switch (shipDirection)
	{
	case 0:
		for (int i = 0; i < shipSize; i++)
		{
			if (!CheckSpot(rowVal - i, colVal, gameKey))
			{
				roomForShip = false;
			}
		}
	case 1:
		for (int i = 0; i < shipSize; i++)
		{
			if (!CheckSpot(rowVal + i, colVal, gameKey))
			{
				roomForShip = false;
			}
		}
	case 2:
		for (int i = 0; i < shipSize; i++)
		{
			if (!CheckSpot(rowVal, colVal - i, gameKey))
			{
				roomForShip = false;
			}
		}
	case 3:
		for (int i = 0; i < shipSize; i++)
		{
			if (!CheckSpot(rowVal, colVal + i, gameKey))
			{
				roomForShip = false;
			}
		}
	}
	return roomForShip;
}

// FUNCTION: randomly places ship on the board, checking to make sure ship stays in bounds
// PARAMETERS: int representing size of the ship, int representing board size, 2D char array for game key,
//             int array pointing to the ship's coordinates
template<const int rows, const int cols, const int x, const int y>
void GenShip(const int SHIP_SIZE, const int BOARD_SIZE, char(&gameKey)[rows][cols], int(&shipCoords)[x][y])
{
	// declare local variables
	int randRow, randCol, randDirection;
	bool placementSuccess = false;

	// convert ship size to appropriate character to represent ship
	map<int, char> sizeToChar = { {2, 'D'}, {3, 'S'}, {4, 'B'}, {5, 'C'} };
	char keyCharacter = sizeToChar[SHIP_SIZE];

	do
	{
		// Generate random starting coordinate between 1 and BOARD_SIZE, inclusive
		randRow = rand() % (BOARD_SIZE - 1) + 1;
		randCol = rand() % (BOARD_SIZE - 1) + 1;

		// 0: UP, 1: DOWN, 2: LEFT, 3: RIGHT
		randDirection = rand() % 4;

		// place ship based on direction and size. Call ShipOverlap to verify if there is room to place.
		// if successful, place the ship and toggle boolean to true, otherwise break reroll the random coordinate.
		// when successful, store the x and y coordinates for each section of the ship in the shipCoords array
		switch (randDirection)
		{
		case 0:
			if ((randRow >= SHIP_SIZE - 1) && ShipOverlap(randRow, randCol, randDirection, SHIP_SIZE, gameKey))
			{
				for (int i = 0; i < SHIP_SIZE; i++)
				{
					gameKey[randRow - i][randCol] = keyCharacter;
					shipCoords[i][0] = randRow - i;
					shipCoords[i][1] = randCol;
				}
				placementSuccess = true;
			}
			break;
		case 1:
			if ((randRow <= BOARD_SIZE - SHIP_SIZE) && ShipOverlap(randRow, randCol, randDirection, SHIP_SIZE, gameKey))
			{
				for (int i = 0; i < SHIP_SIZE; i++)
				{
					gameKey[randRow + i][randCol] = keyCharacter;
					shipCoords[i][0] = randRow + i;
					shipCoords[i][1] = randCol;
				}
				placementSuccess = true;
			}
			break;
		case 2:
			if ((randCol >= SHIP_SIZE - 1) && ShipOverlap(randRow, randCol, randDirection, SHIP_SIZE, gameKey))
			{
				for (int i = 0; i < SHIP_SIZE; i++)
				{
					gameKey[randRow][randCol - i] = keyCharacter;
					shipCoords[i][0] = randRow;
					shipCoords[i][1] = randCol - i;
				}
				placementSuccess = true;
			}
			break;
		case 3:
			if ((randCol <= BOARD_SIZE - SHIP_SIZE) && ShipOverlap(randRow, randCol, randDirection, SHIP_SIZE, gameKey))
			{
				for (int i = 0; i < SHIP_SIZE; i++)
				{
					gameKey[randRow][randCol + i] = keyCharacter;
					shipCoords[i][0] = randRow;
					shipCoords[i][1] = randCol + i;
				}
				placementSuccess = true;
			}
			break;
		}
	} while (placementSuccess == false);
}

// FUNCTION: displays game key as well as the user board when in Debug Mode
// PARAMETERS: int representing board size, 2D char array for game key, ints for score and high score
template<const int rows, const int cols>
void Debug(const int BOARD_SIZE, char(&gameKey)[rows][cols], int currentScore, int highScore)
{
	cout << endl << "          * * * DEBUG KEY * * *" << endl;
	PrintBoard(BOARD_SIZE, gameKey, currentScore, highScore);
}

// FUNCTION: confirms if a ship has been sunk to increment score
// PARAMETERS: int representing the size of the ship to check, 2D char array for game key, 2D int array holding ship coords
// RETURNS: boolean (TRUE if ship is sunk, FALSE if there are still spaces that haven't been hit)
template<const int rows, const int cols, const int x, const int y>
bool ShipSunk(const int SHIP_SIZE, char(&key)[rows][cols], int(&coords)[x][y])
{
	bool shipSunk;

	// iterate over the game key array and ship coord array and check that all corresponding key values are 'X'
	for (int i = 0; i < SHIP_SIZE; i++)
	{
		shipSunk = key[coords[i][0]][coords[i][1]] == 'X';
		if (!shipSunk)
		{
			break;
		}
	}

	return shipSunk;
}

// FUNCTION: calculates and outputs score totals at game over
// PARAMETERS: ints for score, high score, ship bonus score, turns taken, and the length of the game
void GameSummary(int currentScore, int& highScore, int shipBonus, int turnsCount, const int GAME_LENGTH) {
	// calculate speed bonus, ammo bonus, and total score
	int ammoPoints = (GAME_LENGTH - turnsCount) * 25;
	int fastBonus = ((GAME_LENGTH - turnsCount) / 5) * (currentScore + ammoPoints);
	int totalScore = currentScore + fastBonus + ammoPoints;
	
	// Output score summary
	cout << endl << "                  Score Summary" << endl;
	cout << setw(47) << setfill('*') << "" << endl;
	cout << setw(40) << setfill(' ') << left << "* Points for hits:" << setw(5) << right << currentScore - shipBonus << setw(2) << " *" << endl;
	cout << setw(40) << left << "* Points for remaining ammo:" << setw(5) << right << ammoPoints << setw(2) << " *" << endl;
	cout << setw(40) << left << "* Bonus points for destroyed ships:" << setw(5) << right << shipBonus << setw(2) << " *" << endl;
	cout << setw(40) << left << "* Bonus points for efficient victory:" << setw(5) << right << fastBonus << setw(2) << " *" << endl;
	cout << setw(40) << left << "* TOTAL SCORE:" << setw(5) << right << totalScore << setw(2) << " *" << endl;
	cout << setw(47) << setfill('*') << "" << endl << endl;

	// update high score and output if new high score set
	if (totalScore > highScore)
	{
		cout << setw(47) << setfill('*') << "" << endl;
		cout << setw(40) << setfill(' ') << left << "* NEW HIGH SCORE: " << setw(5) << right << totalScore << setw(2) << " *" << endl;
		cout << setw(47) << setfill('*') << "" << endl << endl;
		highScore = totalScore;
	}
}

int main()
{
	// "randomly" generates seed based on system clock. 
	srand(unsigned int(time(NULL)));

	// declare constants
	const int BOARD_SIZE = 11;
	const int CARRIER_SHIP = 5;
	const int BATTLE_SHIP = 4;
	const int SUB_SHIP = 3;
	const int DESTROYER_SHIP = 2;
	const int GAME_LENGTH = 40;
	const int COORD_LEN = 2;
	
	// declare variables
	string shotInput;
	char playAgain;
	char gameKey[BOARD_SIZE][BOARD_SIZE];
	char userBoard[BOARD_SIZE][BOARD_SIZE];
	int highScore = 0;
	int currentScore;
	int shipBonus;
	bool debugMode = false;
	bool destroyerSunk = false;
	bool sub1Sunk = false;
	bool sub2Sunk = false;
	bool carrierSunk = false;
	bool battleshipSunk = false;
	int carrierCoords[CARRIER_SHIP][COORD_LEN];
	int battleCoords[BATTLE_SHIP][COORD_LEN];
	int sub1Coords[SUB_SHIP][COORD_LEN];
	int sub2Coords[SUB_SHIP][COORD_LEN];
	int destroyerCoords[DESTROYER_SHIP][COORD_LEN];

	// let the user play the sickest game of all time
	do
	{
		int turnsCount = 0;
		currentScore = 0;
		shipBonus = 0;

		// initialize game board and populate with random ship(s)
		GenBoard(BOARD_SIZE, gameKey, userBoard);
		GenShip(CARRIER_SHIP, BOARD_SIZE, gameKey, carrierCoords);
		GenShip(BATTLE_SHIP, BOARD_SIZE, gameKey, battleCoords);
		GenShip(SUB_SHIP, BOARD_SIZE, gameKey, sub1Coords);
		GenShip(SUB_SHIP, BOARD_SIZE, gameKey, sub2Coords);
		GenShip(DESTROYER_SHIP, BOARD_SIZE, gameKey, destroyerCoords);

		cout << "    * * *   B A T T L E S H I P   * * *" << endl;
		PrintBoard(BOARD_SIZE, userBoard, currentScore, highScore);

		// user takes shots while they have turns left, with results reported
		while (turnsCount < GAME_LENGTH && !VictoryCheck(BOARD_SIZE, gameKey, userBoard))
		{
			shotInput = UserInput(turnsCount, GAME_LENGTH, debugMode);
			ShotResult(shotInput, BOARD_SIZE, gameKey, userBoard, currentScore, highScore);

			// if debug mode is activate, display game key
			if (debugMode)
			{
				Debug(BOARD_SIZE, gameKey, currentScore, highScore);
			}

			// check for ship sinking and add bonus to score, toggle bool once ship sinks
			if (ShipSunk(DESTROYER_SHIP, gameKey, destroyerCoords) && !destroyerSunk)
			{
				cout << "           Destroyer Sunk: +100" << endl << endl;
				destroyerSunk = true;
				shipBonus += 100;
				currentScore += 100;
			}
			if (ShipSunk(SUB_SHIP, gameKey, sub1Coords) && ShipSunk(SUB_SHIP, gameKey, sub2Coords) && !sub2Sunk)
			{
				cout << "           Submarine Sunk: +150" << endl << endl;
				sub2Sunk = true;
				shipBonus += 150;
				currentScore += 150;
			}
			else if ((ShipSunk(SUB_SHIP, gameKey, sub1Coords) || ShipSunk(SUB_SHIP, gameKey, sub2Coords)) && !sub1Sunk)
			{
				cout << "           Submarine Sunk: +150" << endl << endl;
				sub1Sunk = true;
				shipBonus += 150;
				currentScore += 150;
			}
			if (ShipSunk(BATTLE_SHIP, gameKey, battleCoords) && !battleshipSunk)
			{
				cout << "          Battleship Sunk: +200" << endl << endl;
				battleshipSunk = true;
				shipBonus += 200;
				currentScore += 200;
			}
			if (ShipSunk(CARRIER_SHIP, gameKey, carrierCoords) && !carrierSunk)
			{
				cout << "           Carrier Sunk: +250" << endl << endl;
				carrierSunk = true;
				shipBonus += 250;
				currentScore += 250;
			}
			
			turnsCount++;
		}

		// Game Over scenarios
		if (turnsCount == GAME_LENGTH && !VictoryCheck(BOARD_SIZE, gameKey, userBoard))
		{
			cout << "You've run out of ammunition!" << endl << "You Lose!" << endl;
			GameSummary(currentScore, highScore, shipBonus, turnsCount, GAME_LENGTH);
		}
		else if (!(turnsCount == GAME_LENGTH) && VictoryCheck(BOARD_SIZE, gameKey, userBoard))
		{
			cout << "You've sunk all the ships!" << endl << "You Win!" << endl;
			GameSummary(currentScore, highScore, shipBonus, turnsCount, GAME_LENGTH);
		}
		else
		{
			cout << "You barely sunk all the battleships..." << endl << "How Lucky!" << endl;
			GameSummary(currentScore, highScore, shipBonus, turnsCount, GAME_LENGTH);
		}

		// check to see if user wants to play again, with input validation
		cout << "Would you like to continue? (Y/N): ";
		cin >> playAgain;
		while (!cin || ((playAgain != 'Y') && (playAgain != 'N')))
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Invalid input, please use (Y or N): ";
			cin >> playAgain;
		}

	} while (playAgain == 'Y');

	return 0;
}
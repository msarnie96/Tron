#pragma once

// Tile ascii codes
const char RIGHT_WALL = 'R';
const char LEFT_WALL = 'L';
const char TOP_WALL = 'T';
const char BOTTOM_WALL = 'B';
const char CORNER_WALL = 'C';
const char EMPTY_TILE = ' ';
const char PLAYER_ONE = 'X';
const char PLAYER_TWO = 'O';
const char TRAIL = '*';

class Gameboard
{
public:
	//const char WALL = RIGHT_WALL | LEFT_WALL | TOP_WALL | BOTTOM_WALL;

	// Board constraints
	const int MIN_WIDTH = 10;
	const int MIN_HEIGHT = 10;
	const int MAX_WIDTH = 200;
	const int MAX_HEIGHT = 200;

	// Board
	char** Board;

	// Constructors and destructor
	Gameboard();
	Gameboard(int w, int h);
	//~Gameboard();

	// Function prototypes
	bool Create(int w, int h);
	bool Update(char** updatedBoard);
	void Display();

	char getTile(int x, int y);
	bool setTile(int x, int y, char c);



private:

	int _width;
	int _height;

};
#include "gameboard.h"
#include <iostream>
#include <Windows.h>

Gameboard::Gameboard()
{
	Create(50, 50);
}
Gameboard::Gameboard(int w, int h)
{
	Create(w, h);
}

char Gameboard::getTile(int x, int y)
{
	return Board[y][x];
}

bool Gameboard::setTile(int x, int y, char c)
{
	/* Validate input
	if (x > _width || y > _height || x < 1 || y < 1) 
		return false;
	*/

	if (Board[y][x] == EMPTY_TILE)
	{
		Board[y][x] = c;
		return true;
	}

	return false;
		
}

bool Gameboard::Create(int w, int h)
{
	_width = w;
	_height = h;

	// Check width, add get and set functions later that handle this.
	if (_height < MIN_HEIGHT || _height > MAX_HEIGHT || _width < MIN_WIDTH || _width > MAX_WIDTH)
	{
		return false;
	}

	Board = new char*[_height];

	for (int y = 0; y < _height; ++y)
	{
		Board[y] = new char[_width];

		for (int x = 0; x < _width; ++x)
		{
			if (y == 0 || y == _height -1)
			{
				if (x == 0 || x == _width-1)
					Board[y][x] = CORNER_WALL;
				else if (y == 0)
					Board[y][x] = TOP_WALL;
				else
					Board[y][x] = BOTTOM_WALL;
			}
			else
			{
				if (x == 0)
					Board[y][x] = LEFT_WALL;
				else if (x == _width-1)
					Board[y][x] = RIGHT_WALL;
				else
					Board[y][x] = EMPTY_TILE;
			}
		}
	}
}

bool Gameboard::Update(char** b)
{
	if (sizeof(b) == sizeof(Gameboard))
	{
		delete[] Board; // Free memory
		Board == b;
		return true;
	}
	else
		return false;
}

void Gameboard::Display()
{
	
	if (!Board)
	{
		std::cout << "No board to display... \n";
	}

	for (int y = 0; y < _height; ++y)
	{
		for (int x = 0; x < _width; ++x)
			std::cout << Board[y][x] << "  ";
		
		std::cout << std::endl;
	}
}

void Gameboard::UpdateAt(COORD pos, char tile)
{
	const char *ico = &tile;
	pos.X = pos.X * 3;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf(ico);
}

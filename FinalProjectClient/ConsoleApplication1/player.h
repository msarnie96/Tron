#pragma once

struct playerinfo {
	char username[20];
	int wins;
	int losses;
};

class Player 
{
private:
public:
	playerinfo _profile;
	int x;
	int xPrev;
	int y;
	int yPrev;
	bool isAlive;
};
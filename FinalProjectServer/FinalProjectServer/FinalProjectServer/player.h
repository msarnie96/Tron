#pragma once
#include <string>

struct ProfileInfo
{
	char Username[20];
	int Wins;
	int Losses;
};

class player {
private:
	ProfileInfo _profile;
	bool _isAlive;

public:
	// Constructors
	player();
	//~player();
	player(char*);

	// Position
	int x;
	int y;
	int direction; // Enum defined in GameManager.h

	char Icon;

	void Kill();

	void onWin();
	void onLoss();
	

	char* getName() { return _profile.Username; }
	int getWins() { return _profile.Wins; }
	int getLosses() { return _profile.Losses; }
};
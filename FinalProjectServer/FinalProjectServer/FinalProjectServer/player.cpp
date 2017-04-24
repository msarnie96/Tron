#include <string>
#include <iostream>
#include "player.h"


using namespace std;
player::player()
{
	Icon = 'X';
	_profile = { "Guest", 0, 0};
}
player::player(char* name)
{
	for (int i = 0; name[i]; ++i)
	{
		_profile.Username[i] = name[i];
	}
	_profile.Wins = 0;
	_profile.Losses = 0;
	Icon = 'X';

}

void player::onWin()
{
	//_wins = wins++;
}

void player::onLoss()
{
	//_losses = losses;
}





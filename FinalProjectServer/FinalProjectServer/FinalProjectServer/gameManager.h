#pragma once
#include "gameboard.h"
#include "player.h"

enum Input
{
	UP = 1,
	RIGHT,
	DOWN,
	LEFT
};

class GameManager
{
public:
	GameManager();
	void setBoard(Gameboard*);
	void createBoard(int w, int h);
	int addPlayer(player*);

	bool startGame(); // To do
	void updateGame(); // To do
	bool endGame(); // To do

	void handleInput(player*, Input);
	void movePlayers();

	player* _players[2];

private:
	Gameboard* _board;

};
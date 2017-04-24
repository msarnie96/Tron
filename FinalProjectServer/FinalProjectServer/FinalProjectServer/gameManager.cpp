#include "gameManager.h"

GameManager::GameManager()
{
	_board = 0;
	_players[0] = new player;
	_players[1] = new player;
}

// Assign game board to be used by manager
void GameManager::setBoard(Gameboard* b)
{
	if (&b != nullptr) _board = b;
}

void GameManager::createBoard(int w, int h)
{
	_board = new Gameboard(w, h);
}

// Add a player
int GameManager::addPlayer(player* p)
{
	if (true)
	{
		p->x = 5;
		p->y = 5;
		p->direction = RIGHT;
		//_board->setTile(p->x, p->y, p->Icon);

		_players[0] = p;
		return 1;
	}
	else if (!_players[1])
	{
		p->x = 10;
		p->y = 10;
		p->direction = LEFT;
		//_board->setTile(p->x, p->y, p->Icon);

		_players[1] = p;
		return 2;
	}
	return 0;
}

// Not implemented
bool GameManager::startGame()
{
	return false;
}

void GameManager::updateGame()
{
	_board->Display();
}

bool GameManager::endGame()
{
	return false;
}

void GameManager::handleInput(player* p, Input i)
{
	p->direction = i;
}

// To be cleaned up
void GameManager::movePlayers()
{
	/*
	// Temp position for cleaner movement
	COORD p1Pos;
	COORD p2Pos;
	// Get directions
	int p1Dir = _players[0]->direction;
	int p2Dir = _players[1]->direction;

	p1Pos.X = _players[0]->x;
	p1Pos.Y = _players[0]->y;
	p2Pos.X = _players[1]->x;
	p2Pos.Y = _players[1]->y;

	// Move p1 temp position
	switch (p1Dir)
	{
	case Input::UP:
		--p1Pos.Y;
		break;
	case Input::DOWN:
		++p1Pos.Y;
		break;
	case Input::RIGHT:
		++p1Pos.X;
		break;
	case Input::LEFT:
		--p1Pos.X;
		break;
	}

	// Move p2 temp position
	switch (p2Dir)
	{
	case Input::UP:
		--p2Pos.Y;
		break;
	case Input::DOWN:
		++p2Pos.Y;
		break;
	case Input::RIGHT:
		++p2Pos.X;
		break;
	case Input::LEFT:
		--p2Pos.X;
		break;
	}

	// Check for collision
	if (p1Pos.X == p2Pos.X && p1Pos.Y == p2Pos.Y)
	{
		// Kill both players here
		return;
	}
	
	// Get next tiles
	char p1NextTile = _board->getTile(p1Pos.X, p1Pos.Y);
	char p2NextTile = _board->getTile(p2Pos.X, p2Pos.Y);

	// Check next tile for p1
	switch (p1NextTile)
	{
		// Create trail
		_board->setTile(_players[0]->x, _players[0]->y, TRAIL);
	case EMPTY_TILE:
		// Set position to temp position
		_players[0]->x = p1Pos.X;
		_players[0]->y = p1Pos.Y;
		// Add player to board
		_board->setTile(_players[0]->x, _players[0]->y, _players[0]->Icon);
		break;
	case TRAIL:
	case RIGHT_WALL:
	case LEFT_WALL:
	case TOP_WALL:
	case BOTTOM_WALL:
	case CORNER_WALL:
		//p->Kill();
		break;
	}

	// Create trail
	_board->setTile(_players[0]->x, _players[0]->y, TRAIL);
	_board->setTile(_players[1]->x, _players[1]->y, TRAIL);
	*/
};

	
#pragma once

#pragma pack(push) // save current packing 
#pragma pack(1)    // 1 byte alignment for structs in this file 

// The port number was chosen from dynamic or private ports from 
// https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml
const unsigned short PORT = 49263;
const int UNAME_MAX_LENGTH = 20;

const enum RequestTypes
{
	INP,
	LOGIN,
	JOIN_GAME,
	LEADERBOARDS
};
const enum ResponseTypes
{
	ERR,
	GAMESTATE,
	LOGIN_RESP,
	LEADERBOARD_RESP,
	JOIN_RESP
};
const enum GameStatus {
	LOBBY,
	STARTING,
	IN_PROGRESS,
	ENDED
};

// Header for all structs
struct MSG_HDR {
	int type;
};
struct PLAYER_STATUS {
	int x;
	int y;
	bool isAlive;
	int score;
};
struct LOG_ON {
	MSG_HDR hdr;
	char uname[UNAME_MAX_LENGTH];
};
struct LOG_ON_RESPONSE {
	MSG_HDR hdr;
	bool success;
	char uname[20];
	int wins;
	int losses;
};
struct LB_REQUEST {
	MSG_HDR hdr;
};

struct LB_RESPONSE {
	MSG_HDR hdr;
};
struct JOIN_REQUEST {
	MSG_HDR hdr;
};
struct JOIN_RESPONSE {
	MSG_HDR hdr;
	int playerNum;
};

struct INPUT_REQUEST {
	MSG_HDR hdr;
	int input;
};

struct GAME_RESPONSE {
	// Header
	MSG_HDR hdr;
	// Board
	char buffer[1024];
	int height;
	int width;
	// Player information
	PLAYER_STATUS p1Info;
	PLAYER_STATUS p2Info;

	int gameState;
};


#pragma pack(pop) // back to default packing 
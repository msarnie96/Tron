#pragma once

#ifndef ___BLAH___
#define ___BLAH___

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "player.h"
#include "serverstructs.h"
#include "gameboard.h"
#include <string>

#pragma comment(lib, "Ws2_32.lib")

const int gameboardX = 20;
const int gameboardY = 20;


enum clientstate {
	loggingIn = 0,
	mainMenu,
	quit
};

class gameManager {

public:
	gameManager();
	void start(char* ip);
	int getControls();
	bool game(bool winner, MSG_HDR header, int iResult);
	void HandleResponse();
	Player signUp(Player *player1, MSG_HDR header, int iResult, int state);
	Player logIn(Player *player1, MSG_HDR header, int iResult, int state);
	int leaderboards(Player *player1, MSG_HDR header, int iResult, int state);
	void close();
private:
	WSADATA wsaData;
	SOCKET SendSocket;
	sockaddr_in RecvAddr;


};

#endif ___BLAH___

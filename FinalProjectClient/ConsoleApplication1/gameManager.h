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
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

const int gameboardX = 20;
const int gameboardY = 20;
const int BUFFERSIZE = 2000;


enum clientstate {
	loggingIn = 0,
	mainMenu,
	quit
};

class gameManager {

public:
	gameManager();
	Player playerYou;
	Player playerOther;
	bool winner;
	bool waiting;
	int playernumber = -1;
	int state;
	bool gameOver = false;
	void start(char* ip);
	void joinServerRequest();
	void joinServerResponse(JOIN_RESPONSE* joinresponse);
	int getControls();
	bool sendInput(int input);
	bool listening;
	void listener();
	void operator()(); 
	void draw(GAME_RESPONSE* gameresponse);
	void gameResponse(GAME_RESPONSE* gameresponse);
	bool updateGame();
	void HandleResponse(char* buffer);
	bool signUp(Player playerYou);
	bool logIn(Player playerYou);
	Player logInResponse(LOG_ON_RESPONSE* resp);
	bool getLeaderboards();
	int getLeaderboardsResponse();
	void close();
	void waitforresp();

	WSADATA wsaData;
	SOCKET SendSocket;
	SOCKET RecvSocket;
	sockaddr_in RecvAddr;
	sockaddr_in SendAddr;
private:
};

#endif ___BLAH___

#pragma once

#include <thread>
#include <mutex>
#include "serverstructs.h"
#include "gameManager.h"
#include "Sockets.h"

const int BUFF_LENGTH = sizeof(GAME_RESPONSE);

class Server
{
private:
	
	// Server IP Address
	char* IpAddr = "64.72.2.203";
	char Buffer[BUFF_LENGTH];
	SOCKET ServerSocket;
	SOCKADDR_IN addrServer;
	// Rubber chicken
	std::mutex mtx;
	// WSA vars
	int iWSAStatus;
	WSADATA wsaData;

	bool Listening;
	
	bool HandleInput(INPUT_REQUEST* req, SOCKADDR_IN sender);
	bool LogOn(LOG_ON* req, SOCKADDR_IN sender);
	void SendRespTo(char* respBuf, int size, SOCKADDR_IN target);

	// For testing
	bool showServer();

public:
	// Reference to game manager
	GameManager* Manager;
	
	Server();
	bool start();

	// Threading functions
	void RequestStream();
	
	
	//void InitWinsock();

	//void RequestEnd();
	//void ResponseEnd();
};
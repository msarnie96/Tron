#include "server.h"
#include <iostream>

Server::Server()
{
}

// setsockopt, SOBROADCAST
bool Server::start()
{
	Listening = true;

	// Init server addr
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// Start windows socket api version 2.2
	iWSAStatus = WSAStartup(MAKEWORD(2, 2), &wsaData);

	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Validate socket creation
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << "Could not create socket. Closing program..." << std::endl;
		WSACleanup();

		return false; // Failed
	}

	int nRet;

	// Bind socket to server address
	bind(ServerSocket, (LPSOCKADDR)&addrServer, sizeof(addrServer));

	/*
	// Validate socket bind
	if (nRet == SOCKET_ERROR)
	{
		std::cout << "ERROR binding! " << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return false;
	}
	*/

	// Test the server
	if (!showServer())
	{
		std::cout << "ERROR showing server: " << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return false;
	}

	return true;
}

// Show the server
bool Server::showServer()
{
	char szSvr[256];
	int nRet;

	nRet = gethostname(szSvr, sizeof(szSvr));

	if (nRet == SOCKET_ERROR)
	{
		return false;
	}

	std::cout << "Server " << szSvr << " waiting on port " << PORT << std::endl;

	return true;
}

void Server::RequestStream()
{
	while (Listening)
	{
		bool error = false;

		//Connected address
		SOCKADDR_IN sender_addr;
		// Address length
		int sender_addr_len = sizeof(sender_addr);
		int nRet;

		// Recieve Packet
		nRet = recvfrom(ServerSocket, Buffer, BUFF_LENGTH, 0, (LPSOCKADDR)&sender_addr, &sender_addr_len);
		MSG_HDR* hdr = (MSG_HDR*)Buffer;

		switch (hdr->type)
		{
		case LOGIN:
			LOG_ON* logonReq = (LOG_ON*)Buffer;
			break;
		case INPUT:
			INPUT_REQUEST* inputReq = (INPUT_REQUEST*)Buffer;
			break;
		case JOIN_GAME:

			break;
		default:
			break;
		}
	}
}

bool Server::HandleInput(INPUT_REQUEST * req, SOCKADDR_IN sender)
{
	
	return false;
}

bool Server::LogOn(LOG_ON * req, LOG_ON_RESPONSE * res, SOCKADDR_IN sender)
{
	player* Player = new player(sender, req->uname);
	Manager.
	return false;
}

void Server::sendResponse(char * buff, int size)
{
}
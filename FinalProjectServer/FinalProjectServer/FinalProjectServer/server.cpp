#include "server.h"
#include <iostream>

Server::Server()
{
	Manager = new GameManager;
}

// Start server
bool Server::start()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData); // Start WSA

	ServerSocket = SOCKET();
	Listening = true;

	// Init server addr
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServer.sin_port = htons(PORT);
	

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
	nRet = bind(ServerSocket, (LPSOCKADDR)&addrServer, sizeof(addrServer));

	
	// Validate socket bind
	if (nRet == SOCKET_ERROR)
	{
		std::cout << "ERROR binding! " << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return false;
	}

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
	if (Listening) std::cout << "Listening for requests on port " << PORT << std::endl;
	else std::cout << "Unable to start listening..." << std::endl;
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
		if (nRet == SOCKET_ERROR)
		{
			std::cout << "Error : " << WSAGetLastError() << std::endl;
			break;
		}
		
		MSG_HDR* hdr = (MSG_HDR*)Buffer;

		switch (hdr->type)
		{
		case LOGIN:
		{
			std::cout << "Logon request recieved." << std::endl;
			LOG_ON* logonReq = (LOG_ON*)Buffer;
			LogOn(logonReq, sender_addr);
		}
			break;
		case INP:
		{
			INPUT_REQUEST* inputReq = (INPUT_REQUEST*)Buffer;
		}
			break;
		case JOIN_GAME:

			break;
		default:
			break;
		}
	}
}

bool Server::HandleInput(INPUT_REQUEST* req, SOCKADDR_IN sender)
{
	


	return false;
}

bool Server::LogOn(LOG_ON* req, SOCKADDR_IN sender)
{
	player* Player = new player(req->uname);
	
	if (Player == nullptr) return false; // Validate player

	// Load SQL profile information here
	Manager->addPlayer(Player);
	char* uName = Player->getName();

	// Prepare response
	LOG_ON_RESPONSE* response = new LOG_ON_RESPONSE;
	response->hdr = { LOGIN_RESP }; // Resp Header

	// Get uName from char*
	for (int i = 0; i < UNAME_MAX_LENGTH; ++i)
	{
		try {
			response->uname[i] = uName[i];
		}
		catch(std::exception e)
		{
			std::cout << e.what() << std::endl;
			break;
		}
	}

	// Set wins and losses
	response->wins = Player->getWins();
	response->losses = Player->getLosses();

	SendRespTo((char*)response, sizeof(LOG_ON_RESPONSE), sender);

	return true;
}

void Server::SendRespTo(char* buff, int size, SOCKADDR_IN target)
{
	int r;
	// Send response
	r = sendto(ServerSocket, buff, size, 0, (LPSOCKADDR)&target, sizeof(target));
	if (r == SOCKET_ERROR)
	{
		std::cout << "Error sending response. " << WSAGetLastError() << std::endl;
	}
	else
		std::cout << "Response sent successfully." << std::endl;

	delete[] buff;
}

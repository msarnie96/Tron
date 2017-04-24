#include "ServerUDP.h"
#include "ServerHead.h"
#include "gameManager.h"
#include <string>
#include <conio.h>
#include <string>
#include <iostream>
/*
void connectToServer(INPUT_REQUEST packetToSend, string ip)
{
	cout << "IP: " << ip;

	int iResult;
	WSADATA wsaData;
	SOCKET SendSocket = INVALID_SOCKET;
	sockaddr_in RecvAddr;
	int RecvAddrSize = sizeof(RecvAddr);

	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET)
	{
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.1.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT);
	inet_pton(RecvAddr.sin_family, ip.c_str(), &RecvAddr.sin_addr);


	wprintf(L"\nSending a datagram to the receiver...\n");

	iResult = sendto(SendSocket,
		(char*)&packetToSend,
		sizeof(packetToSend),
		0,
		(SOCKADDR *)& RecvAddr,
		sizeof(RecvAddr));

	INPUT_RESPONSE response;

	iResult = recvfrom(
		SendSocket,
		(char*)&response,
		sizeof(response),
		0,
		(SOCKADDR *)& RecvAddr,
		&RecvAddrSize);

	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		return;
	}

	// When the application is finished sending, close the socket.
	wprintf(L"Finished sending. \nClosing socket.\n");
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	//---------------------------------------------
	// Clean up and quit.
	wprintf(L"Exiting.\n\n");
}
*/

void Server::RequestInput(INPUT_REQUEST packetToSend, std::string ip)
{
	std::cout << "IP: " << ip;

	int iResult;

	SOCKET SendSocket = INVALID_SOCKET;
	sockaddr_in RecvAddr;
	int RecvAddrSize = sizeof(RecvAddr);

	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET)
	{
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.1.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT);
	inet_pton(RecvAddr.sin_family, ip.c_str(), &RecvAddr.sin_addr);


	wprintf(L"\nSending a datagram to the receiver...\n");

	iResult = sendto(SendSocket,
		(char*)&packetToSend,
		sizeof(packetToSend),
		0,
		(SOCKADDR *)& RecvAddr,
		sizeof(RecvAddr));

	INPUT_RESPONSE response;

	iResult = recvfrom(
		SendSocket,
		(char*)&response,
		sizeof(response),
		0,
		(SOCKADDR *)& RecvAddr,
		&RecvAddrSize);

	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		return;
	}

	// When the application is finished sending, close the socket.
	wprintf(L"Finished sending. \nClosing socket.\n");
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	//---------------------------------------------
	// Clean up and quit.
	wprintf(L"Exiting.\n\n");
}

void Server::ResponseInput()
{
	// Socket and response vars
	SOCKET ServerSocket;
	INPUT_REQUEST RecievedData;
	INPUT_RESPONSE Response;

	// Windows Socket API Vars
	int iWSAStatus;
	WSADATA wsaData;

	// Server Address Struct
	SOCKADDR_IN addrServer;
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

		return;
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
		return;
	}
	// Test the server
	if (!showServer())
	{
		std::cout << "ERROR showing server: " << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}
	/*
	// Listen for incoming client connections.
	if (listen(ServerSocket, MAX_PENDING_CONNECTS) == SOCKET_ERROR)
	{
	cout << "ERROR listen: " << WSAGetLastError() << endl;
	closesocket(ServerSocket);
	WSACleanup();
	return 1;
	}
	*/


	bool flag = true;

	while (!_kbhit())
	{
		// reset flag
		flag = true;

		// Connected address
		SOCKADDR_IN sender_addr;
		// Address length
		int sender_addr_len = sizeof(sender_addr);

		// Recieve Packet
		nRet = recvfrom(ServerSocket, (char*)& RecievedData, sizeof(INPUT_REQUEST), 0, (LPSOCKADDR)&sender_addr, &sender_addr_len);

		if (nRet == SOCKET_ERROR)
		{
			flag = Error(&RecievedData, &Response);
			std::cout << "Error recieving packet. " << WSAGetLastError() << std::endl;

			// Send response
			nRet = sendto(ServerSocket, (char*)& Response, sizeof(INPUT_RESPONSE), 0, (LPSOCKADDR)&sender_addr, sender_addr_len);
			if (nRet == SOCKET_ERROR)
			{
				std::cout << "Error sending error (lol). " << WSAGetLastError() << std::endl;
			}

			// Clean up and close loop
			closesocket(ServerSocket);
			WSACleanup();
			break; // Break from loop
		}

		// Switch on operation type
		switch (RecievedData.type)
		{
		case TYPE_INPUT:
			std::cout << "Input Request sent from server." << std::endl;
			break;
		case TYPE_ERROR:
			std::cout << "Cannot calculate for type 'error'" << std::endl;
			break;
		default:
			flag = Error(&RecievedData, &Response);
		}

		// Send response
		nRet = sendto(ServerSocket, (char*)& Response, sizeof(INPUT_RESPONSE), 0, (LPSOCKADDR)&sender_addr, sender_addr_len);
		if (nRet == SOCKET_ERROR)
		{
			std::cout << "Error sending response. " << WSAGetLastError() << std::endl;
		}
		else
			std::cout << "Response sent successfully." << std::endl;

	}
	return;
}

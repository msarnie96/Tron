#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "ConsoleApplication1\serverstructs.h"
#include "../ConsoleApplication1/ConsoleApplication1/Intro.h"
#include "ConsoleApplication1\player.h"
#include "ConsoleApplication1\gameboard.h"

using namespace std;

const int gameboardX = 20;
const int gameboardY = 20;

enum direction {
	UP = 1,
	RIGHT,
	DOWN,
	LEFT
};

enum clientstate {
	loggingIn = 0,
	mainMenu,
	quit
};


#pragma comment(lib, "Ws2_32.lib")

int leaderboards(Player *player1, MSG_HDR header, int iResult, int state, SOCKET SendSocket, sockaddr_in RecvAddr)
{
	char buffer[2000];
	SetConsoleTitle(TEXT("|TRON| Leaderboards"));
	//header.type = LEADERBOARDS;
	//ping the server
	//retrieve info from database
	//Display top 10 players win/loss ratios
	system("pause");
	return 0;
}
int getControls()
{
	if(GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000)
		{
			return UP;
		}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
		{
			return RIGHT;
		}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000)
		{
			return DOWN;
		}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000)
		{
			return LEFT;
		}
	return NULL;
}
void HandleResponse()
{
	char buffer[1024];
	MSG_HDR* hdr = (MSG_HDR*)buffer;

	while (true)
	{
		switch (hdr->type)
		{
		case RequestTypes::LOGIN:
			break;
		case RequestTypes::INP:
			break;
		case RequestTypes::JOIN_GAME:
			break;
		default:
			break;
		}
	}
}

bool game(bool winner, MSG_HDR header, int iResult, SOCKET SendSocket, sockaddr_in RecvAddr)
{
	system("cls");
	int input = NULL;
	bool gameOver = false;
	INPUT_REQUEST sendcontroller{ header, input };
	Gameboard myGameboard = Gameboard(gameboardX,gameboardY);
	myGameboard.Display();
	while (!gameOver)
	{
		//Get Game Controls
		input = getControls();
		//Send controller input to server
		iResult = sendto(SendSocket,
			(char*)&sendcontroller, sizeof(sendcontroller), 0, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
		if (iResult == SOCKET_ERROR) {
			wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			WSACleanup();
			return 1;
		}
		//Wait to recv info from server

		//Save info from server

		//Print out the game board
		
	}
	
	return winner;
}
Player signUp(Player *player1, MSG_HDR header, int iResult, int state,	SOCKET SendSocket, sockaddr_in RecvAddr)
{
	while (state == loggingIn)
	{
		LOG_ON_RESPONSE* logonresponse = new LOG_ON_RESPONSE;
		char buffer[2000];
		bool invalidUName = false;
		int sizeOfName = 0;
		SetConsoleTitle(TEXT("|TRON| Sign Up"));
		system("cls");
		cout << ">Creating new username... Please use only letters and numbers" << endl;
		cout << ">Enter Desired Username: ";
		cin.getline(player1->_profile.username, 20); //Get users desired username
		while (player1->_profile.username[sizeOfName] != NULL)
		{
			if (player1->_profile.username[sizeOfName] == '!') //Check if username uses invalid characters
			{
				cout << ">Exclamation marks are not an allowed character" << endl;
				invalidUName = true;
			}
			sizeOfName++;
		}
		//Send Server Username - Signing Up
		if (invalidUName == false)
		{
			header.type = LOGIN; //We are sending the server a login request
			LOG_ON user{ header, *player1->_profile.username };//Create the request
			for (int i = 0; i < 20; i++)
			{
				user.uname[i] = player1->_profile.username[i];
			}
			iResult = sendto(SendSocket, //Send Request
				(char*)&user, sizeof(user), 0, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
			if (iResult == SOCKET_ERROR) {
				wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
				closesocket(SendSocket);
				WSACleanup();
			}
			//logonresponse->success = true; //Temp until we start receiving stuff

			//Wait & Recv validation username is not taken
			iResult = recvfrom(SendSocket, //Receive Request back from server
				(char*)&logonresponse, sizeof(logonresponse), 0, NULL, NULL);
			if (iResult == SOCKET_ERROR) {
				wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
			}
			logonresponse = (LOG_ON_RESPONSE*)buffer;
			//Check if login was successful
			if (logonresponse->success == false)
			{
				cout << "Username is taken... Please choose another" << endl;
				system("pause");
			}
			else
			{
				state = mainMenu;
			}
		}
		else 
		{
			system("pause");
		}
	}
	
	return *player1;
}
Player logIn(Player *player1, MSG_HDR header, int iResult, int state, SOCKET SendSocket, sockaddr_in RecvAddr)
{
	LOG_ON_RESPONSE* logonresponse = new LOG_ON_RESPONSE;
	char buffer[2000];
	while (state == loggingIn)
	{
		bool invalidUName = false;
		//Send Server Username - Logging In
		header.type = LOGIN; //We are sending the server a login request
		LOG_ON user{ header, *player1->_profile.username }; //Create the request
		for (int i = 0; i < 20; i++)
		{
			user.uname[i] = player1->_profile.username[i];
		}
		iResult = sendto(SendSocket, //Send Request
			(char*)&user, sizeof(user), 0, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
		if (iResult == SOCKET_ERROR) {
			wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			WSACleanup();
		}
		//Wait & Recv login respons
		iResult = recvfrom(SendSocket, //Receive Request back from server
			buffer, sizeof(LOG_ON_RESPONSE), 0, NULL, NULL);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		}
		logonresponse = (LOG_ON_RESPONSE*)buffer;
		//logonresponse->success = true; //Temp until we start receiving stuff
		//Check if login was successful
		if (logonresponse->success == false)
		{
			cout << ">This username does not exist... bringing you to  the sign up menu." << endl;
			system("pause");
			*player1 = signUp(player1, header, iResult, state, SendSocket, RecvAddr);
		}
		else
		{
			state = mainMenu;
		}
	}
	return *player1;
}

int main()
{
	SetConsoleTitle(TEXT("|TRON|"));
	WSADATA wsaData;
	int iResult = 0;
	SOCKET SendSocket = INVALID_SOCKET;
	sockaddr_in RecvAddr;

	//Intro intro;
	//intro.Play();
	system("color 0A");
	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	
	MSG_HDR header{ ERR };
	
	Player *player1 = new Player();

	bool invalidUName = false;
	bool winner = false;
	char input[100];
	int state = loggingIn;
	
	
	//--------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.1.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT);
	RecvAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	SetConsoleTitle(TEXT("|TRON| Login "));


	cout << ">Type '!' to create a new account." << endl << endl;
	cout << ">Enter Username: ";
	cin.getline(player1->_profile.username, 20); //Get username to either sign up or log in
	if (player1->_profile.username[0] == '!') //User wants to make new account
	{
		*player1 = signUp(player1, header, iResult, state, SendSocket, RecvAddr);
	}
	else
	{
		*player1 = logIn(player1, header, iResult, state, SendSocket, RecvAddr);
	}
	state = mainMenu;

	while (state == mainMenu)
	{
		system("cls");
		SetConsoleTitle(TEXT("|TRON| Main Menu: "));
		cout << ">Welcome: " << player1->_profile.username << endl << ">Type any of the commands below." << endl << endl << ">Find Game" << endl << ">My Stats " << endl << ">Leaderboards" << endl << ">Quit" << endl << endl;
		cin.getline(input, 100);
		if (input[0] == 'f' || input[0] == 'F')
		{
			SetConsoleTitle(TEXT("|TRON| In Game "));
			winner = game(winner, header, iResult, SendSocket, RecvAddr); //Start up the game, and return of they won or not
			//Get end game info and update wins / losses
			if (winner == true)
			{
				player1->_profile.wins++;
			}
			else if (winner == false)
			{
				player1->_profile.losses++;
			}
			else
			{
				//Somehow broken how booleans work
				cout << "Boolean is neither true nor false" << endl;
				system("pause");
			}
		}
		else if (input[0] == 'm' || input[0] == 'M')
		{
			system("cls");
			cout << ">" << player1->_profile.username << endl;
			cout << ">Wins: " << player1->_profile.wins << endl;
			cout << ">Losses: " << player1->_profile.losses << endl;
			system("pause");
		}
		else if (input[0] == 'l' || input[0] == 'L')
		{
			leaderboards(player1, header, iResult, state, SendSocket, RecvAddr);
		}
		else if (input[0] == 'q' || input[0] == 'Q')
		{
			state = quit;
		}
		else
		{
			cout << ">Invalid command..." << endl;
			system("pause");
		}
	}
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	wprintf(L"Finished sending. Closing socket.\n");
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//---------------------------------------------
	// Clean up and quit.
	wprintf(L"Exiting.\n");
	WSACleanup();
	delete[] player1;
	return 1;
}

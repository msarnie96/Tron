#include "gameManager.h"
using namespace std;

gameManager::gameManager()
{
}

void  gameManager::HandleResponse()
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

void gameManager::start(char * ip)
{
	int iResult = 0;
	SendSocket = INVALID_SOCKET;
	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
	}

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
	}
	//--------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.1.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_addr.s_addr = InetPton(AF_INET, (PCWSTR)ip, &(RecvAddr));
	RecvAddr.sin_port = htons(PORT);

}


int  gameManager::getControls()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000)
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
	return -1;
}

bool  gameManager::game(bool winner, MSG_HDR header, int iResult)
{
	char buffer[2000];
	system("cls");
	int input = NULL;
	bool gameOver = false;
	INPUT_REQUEST sendcontroller{ header, input };
	Gameboard myGameboard = Gameboard(gameboardX, gameboardY);
	myGameboard.Display();
	while (!gameOver)
	{
		//Get Game Controls
		input = getControls();
		//Send controller input to server
		if (input != -1)
		{
			iResult = sendto(SendSocket,
				(char*)&sendcontroller, sizeof(sendcontroller), 0, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
			if (iResult == SOCKET_ERROR) {
				wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
				closesocket(SendSocket);
				WSACleanup();
				return 1;
			}
		}

		//Wait to recv info from server
		iResult = recvfrom(SendSocket, //Receive Request back from server
			buffer, sizeof(GAME_RESPONSE), 0, NULL, NULL);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		}
		//Save info from server

		//Print out the game board

	}

	return winner;
}

int  gameManager::leaderboards(Player *player1, MSG_HDR header, int iResult, int state)
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



Player  gameManager::signUp(Player *player1, MSG_HDR header, int iResult, int state)
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
Player  gameManager::logIn(Player *player1, MSG_HDR header, int iResult, int state)
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
			*player1 = signUp(player1, header, iResult, state);
		}
		else
		{
			state = mainMenu;
		}
	}
	return *player1;
}

void gameManager::close()
{
	int iResult;
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
	}
	//---------------------------------------------
	// Clean up and quit.
	wprintf(L"Exiting.\n");
	WSACleanup();
}
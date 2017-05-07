#include "gameManager.h"
//using namespace std;

std::mutex mtx;

gameManager::gameManager()
{
}

void gameManager::listener()
{
	while (listening) 
	{
		int iResult = 0;
		char  buffer[2000];
		iResult = bind(RecvSocket, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
		iResult = recvfrom(RecvSocket, //Send Request
			buffer, BUFFERSIZE, 0, NULL, NULL);
		HandleResponse(buffer);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"sendto failed with error at listener: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			WSACleanup();
		}
		
			
	}

}
//Finished
void gameManager::operator()()
{
	listener();
}
//Finished
void gameManager::gameResponse(GAME_RESPONSE* gameresponse)
{
	playerYou.xPrev = playerYou.x;
	playerYou.xPrev = playerYou.y;
	if (playerYou._profile.playerId == 1)
	{
		playerYou.x = gameresponse->p1Info.x;
		playerYou.y = gameresponse->p1Info.y;
	}
	else if (playerYou._profile.playerId == 2)
	{
		playerYou.x = gameresponse->p2Info.x;
		playerYou.y = gameresponse->p2Info.y;
	}
	
	if (gameresponse->gameState == ENDED)
	{
		gameOver = true;
		if (playernumber == -1)
		{
			std::cout << "Never received player number?" << std::endl;
			system("pause");
		}
		else if (playernumber == 1) //Are you player one?
		{
			if (gameresponse->p1Info.isAlive == false) //You have died
			{
				winner = false;
			}
			else if (gameresponse->p2Info.isAlive == false)//Enemy has died
			{
				winner = true;
			}
		}
		else if (playernumber == 2)//Are you player two?
		{
			if (gameresponse->p1Info.isAlive == false) //Enemy has Died
			{
				winner = true;
			}
			else if (gameresponse->p2Info.isAlive == false) // you have died
			{
				winner = false;
			}
		}
	}


	draw(gameresponse);
}
//Finished
void  gameManager::HandleResponse(char* buffer)
{
	MSG_HDR* header = (MSG_HDR*)buffer;

	switch (header->type)
	{
	case ResponseTypes::LOGIN_RESP:
	{
		LOG_ON_RESPONSE* resp = (LOG_ON_RESPONSE*)buffer;
		playerYou = logInResponse(resp);
		break;
	}

	case ResponseTypes::GAMESTATE:
	{
		GAME_RESPONSE* gameresponse = (GAME_RESPONSE*)buffer;
		gameResponse(gameresponse);
		break;
	}
	case ResponseTypes::JOIN_RESP:
	{
		JOIN_RESPONSE* joinresponse = (JOIN_RESPONSE*)buffer;
		joinServerResponse(joinresponse);
		break;
	}
	case ResponseTypes::ERR:
	{
		break;
	}
	default:
		break;

	}
}
//Finished
Player gameManager::logInResponse(LOG_ON_RESPONSE* resp)
{
	if (resp->hdr.type == ResponseTypes::ERR)
	{
		std::cout << "You have entered an invalid username, please try again..." << std::endl;
		system("pause");
	}
	else
	{
		playerYou._profile.losses = resp->losses;
		playerYou._profile.wins = resp->wins;
		state = mainMenu;
	}
	waiting = false;
	return playerYou;
}
//Finished

void gameManager::start(char * ip)
{
	int iResult = 0;
	SendSocket = INVALID_SOCKET;
	RecvSocket = INVALID_SOCKET;
	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error at startup: %d\n", iResult);
	}
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error at startup: %ld\n", WSAGetLastError());
		WSACleanup();
	}
	//--------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.1.1")
	// and the specified port number.
	SendAddr.sin_family = AF_INET;
	int pton;
	u_char b1 = 64;
	u_char b2 = 72;
	u_char b3 = 1;
	u_char b4 = 230;
	SendAddr.sin_addr.S_un.S_un_b.s_b1 = b1;
	SendAddr.sin_addr.S_un.S_un_b.s_b2 = b2;
	SendAddr.sin_addr.S_un.S_un_b.s_b3 = b3;
	SendAddr.sin_addr.S_un.S_un_b.s_b4 = b4;
	pton = InetPton(AF_INET, (PCWSTR)ip, &(SendAddr.sin_addr.S_un));

	SendAddr.sin_port = htons(PORT);

	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);


}
//Finished
void gameManager::joinServerRequest()
{
	MSG_HDR header;
	header.type = JOIN_GAME;
	JOIN_REQUEST joinrequest{ header };
	int iResult = 0;
	iResult = sendto(SendSocket, //Send Request
		(char*)&joinrequest, sizeof(joinrequest), 0, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
}
//Finished

void gameManager::joinServerResponse(JOIN_RESPONSE* joinresponse)
{
	playernumber = joinresponse->playerNum;
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
//Finished
bool gameManager::sendInput(int input)
{
	MSG_HDR header;
	int iResult = 0;
	header.type = INP;
	INPUT_REQUEST inputreq = { header, input };

	mtx.lock();
	iResult = sendto(SendSocket, //Send Request
		(char*)&inputreq, sizeof(inputreq), 0, (SOCKADDR *)& SendAddr, sizeof(SendAddr));
	mtx.unlock();
	return 0;
}
//Finished
bool  gameManager::updateGame()
{
	winner = false;
	system("cls");
	int lastdirection = -1;
	int input = NULL;
	gameOver = false;
	while (!gameOver)
	{
		//Get Game Controls
		input = getControls();

		//Send controller input to server
		if (lastdirection != input)
		{ 
			sendInput(input);
		}
		lastdirection = input;

		
	}

return winner;
}
//Finished
void gameManager::draw(GAME_RESPONSE* gameresponse)
{
	system("cls");
	int count = gameresponse->height;
	while (count != 0)
	{
		for (int zz = 0; zz <= gameresponse->width; zz++)
		{
			std::cout << gameresponse->buffer[zz];
		}
		std::cout << std::endl;
	}
}
//Finished
bool  gameManager::getLeaderboards()
{
	SetConsoleTitle(TEXT("|TRON| Leaderboards"));
	MSG_HDR header;
	header.type = LEADERBOARDS;
	int iResult = 0;
	LB_REQUEST lbrequest = { header };
	mtx.lock();
	iResult = sendto(SendSocket, //Send Request
		(char*)&lbrequest, sizeof(LB_REQUEST), 0, (SOCKADDR *)& SendAddr, sizeof(SendAddr));
	mtx.unlock();
	if (iResult == SOCKET_ERROR) {
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
	}
	

	system("pause");
	return 0;
}
//Unimplemeted
bool  gameManager::signUp(Player playerYou)
{
	int iResult = 0;
	MSG_HDR header;
	bool invalidUName = false;
	int sizeOfName = 0;
	SetConsoleTitle(TEXT("|TRON| Sign Up"));
	system("cls");
	std::cout << ">Creating new username... Please use only letters and numbers" << std::endl;
	std::cout << ">Enter Desired Username: ";
	std::cin.getline(playerYou._profile.username, 20); //Get users desired username
	while (playerYou._profile.username[sizeOfName] != NULL)
	{
		if (playerYou._profile.username[sizeOfName] == '!') //Check if username uses invalid characters
		{
			std::cout << ">Exclamation marks are not an allowed character" << std::endl;
			invalidUName = true;
		}
		sizeOfName++;
	}
	//Send Server Username - Signing Up
	if (invalidUName == false)
	{
		header.type = LOGIN; //We are sending the server a login request
		LOG_ON user{ header, *playerYou._profile.username };//Create the request
		for (int i = 0; i < 20; i++)
		{
			user.uname[i] = playerYou._profile.username[i];
		}
		mtx.lock();
		iResult = sendto(SendSocket, //Send Request
			(char*)&user, sizeof(user), 0, (SOCKADDR *)& SendAddr, sizeof(SendAddr));
		mtx.unlock();
		if (iResult == SOCKET_ERROR) {
			wprintf(L"sendto failed with error at signup: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			WSACleanup();
		}
		

	}

	return true;
}
//Finished
bool  gameManager::logIn(Player playerYou)
{
	int iResult = 0;
	MSG_HDR header;
	bool invalidUName = false;
	//Send Server Username - Logging In
	header.type = LOGIN; //We are sending the server a login request
	LOG_ON user{ header, *playerYou._profile.username }; //Create the request
	for (int i = 0; i < 20; i++)
	{
		user.uname[i] = playerYou._profile.username[i];
	}
	//mtx.lock();
	iResult = sendto(SendSocket, //Send Request
		(char*)&user, sizeof(user), 0, (SOCKADDR *) &SendAddr, sizeof(SendAddr));
	//mtx.unlock();
	if (iResult == SOCKET_ERROR) {
		wprintf(L"sendto failed with error at log in: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();

	}
	return true;
}
//Finished
int gameManager::getLeaderboardsResponse()
{
	return 0;
}
//Unimplimented
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

void gameManager::waitforresp()
{
	playernumber = -1;
	while (playernumber == -1)
	{

	}
}

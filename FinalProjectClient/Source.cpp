#include "ConsoleApplication1\\gameManager.h" // SERVER_IP found in here 
#include "../FinalProjectClient/ConsoleApplication1/Intro.h"
#include <thread>


using namespace std;

const char* SERVER_IP = "64.72.1.230";

int main()
{
	char IP[20];
	for (int p = 0; p < sizeof(SERVER_IP); p++)
	{
		IP[p] = SERVER_IP[p];
	}
	
	int iResult = 0;
	gameManager gameManager;
	gameManager.start(IP);
	thread handleResp(gameManager);
	
	

	SetConsoleTitle(TEXT("|TRON|"));
	//Intro intro;
	//intro.Play();
	system("color 0A");
	
	
	MSG_HDR header{ ERR };
	Player *playerYou = new Player();

	
	bool invalidUName = false;
	bool winner = false;
	char input[100];
	gameManager.state = loggingIn;
	gameManager.waiting = true;

	SetConsoleTitle(TEXT("|TRON| Login "));
	while (gameManager.state == loggingIn)
	{
		cout << ">Type '!' to create a new account." << endl << endl;
		cout << ">Enter Username: ";
		cin.getline(playerYou->_profile.username, 20); //Get username to either sign up or log in
		if (playerYou->_profile.username[0] == '!') //User wants to make new account
		{
			gameManager.signUp(*playerYou);
		}
		else
		{
			gameManager.logIn(*playerYou);
		}
		cout << ">Loading... " << endl;
		while (gameManager.waiting == true)
		{

		}
	}


	while (gameManager.state == mainMenu)
	{
		system("cls");
		SetConsoleTitle(TEXT("|TRON| Main Menu: "));
		cout << ">Welcome: " << playerYou->_profile.username << endl << ">Type any of the commands below." << endl << endl << ">Find Game" << endl << ">My Stats " << endl << ">Quit" << endl << endl;
		cin.getline(input, 100);
		if (input[0] == 'f' || input[0] == 'F')
		{
			gameManager.joinServerRequest();
			gameManager.waitforresp(); //We have to wait here til we get a response as to be sure we connect to the server

			SetConsoleTitle(TEXT("|TRON| In Game "));
			winner = gameManager.updateGame(); //Start up the game, and return of they won or not
			//Get end game info and update wins / losses
			if (winner == true)
			{
				playerYou->_profile.wins++;
			}
			else if (winner == false)
			{
				playerYou->_profile.losses++;
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
			cout << ">" << playerYou->_profile.username << endl;
			cout << ">Wins: " << playerYou->_profile.wins << endl;
			cout << ">Losses: " << playerYou->_profile.losses << endl;
			system("pause");
		}
		else if (input[0] == 'q' || input[0] == 'Q')
		{
			gameManager.state = quit;
		}
		else
		{
			cout << ">Invalid command..." << endl;
			system("pause");
		}
	}
	gameManager.close();
	return 1;
}

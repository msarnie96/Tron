#include "ConsoleApplication1\\gameManager.h" // SERVER_IP found in here 
#include "../FinalProjectClient/ConsoleApplication1/Intro.h"
#include <thread>
#include <mutex>

using namespace std;

const char* SERVER_IP = "64.72.2.203";

int main()
{
	char IP = *SERVER_IP;
	int iResult = 0;
	gameManager gameManager;
	thread handleResp;
	mutex mtx;
	gameManager.start(&IP);

	SetConsoleTitle(TEXT("|TRON|"));
	//Intro intro;
	//intro.Play();
	system("color 0A");

	
	MSG_HDR header{ ERR };
	Player *player1 = new Player();

	bool invalidUName = false;
	bool winner = false;
	char input[100];
	int state = loggingIn;

	SetConsoleTitle(TEXT("|TRON| Login "));
	cout << ">Type '!' to create a new account." << endl << endl;
	cout << ">Enter Username: ";
	cin.getline(player1->_profile.username, 20); //Get username to either sign up or log in
	if (player1->_profile.username[0] == '!') //User wants to make new account
	{
		*player1 = gameManager.signUp(player1, header, iResult, state);
	}
	else
	{
		*player1 = gameManager.logIn(player1, header, iResult, state);
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
			winner = gameManager.game(winner, header, iResult); //Start up the game, and return of they won or not
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
			gameManager.leaderboards(player1, header, iResult, state);
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
	gameManager.close();
	return 1;
}

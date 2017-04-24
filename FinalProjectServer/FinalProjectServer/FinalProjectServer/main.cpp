#include <iostream>
#include <conio.h>
#include "gameboard.h"
#include "player.h"
#include "server.h"

using namespace std;

int main()
{
	// Init server and manager
	//Server server;
	//GameManager Manager;
	// Set reference to manager
	//server.Manager = &Manager;

	// Start server
	//server.start();

	//thread mainThread(server.RequestStream);

	Server server;
	bool s = server.start();
	server.RequestStream();

	system("pause");

	return 0;
}
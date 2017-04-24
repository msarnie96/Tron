#pragma once
#include "ServerUDP.h"
#include <iostream>
#include <string>

// Show the server
bool showServer()
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

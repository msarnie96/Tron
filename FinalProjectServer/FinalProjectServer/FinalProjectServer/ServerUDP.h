#pragma once

#include <WinSock2.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <string>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#pragma pack(push) // save current packing 
#pragma pack(1)    // 1 byte alignment for structs in this file 




// The port number was chosen from dynamic or private ports from 
// https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml
const unsigned short PORT = 49263;

// type field values
const char TYPE_INPUT = 1;


// The server will respond with an error message if:
// - count is less than 2 or greater than 5
// - The type is not valid 
// - something goes wrong in the processing of data 
const char TYPE_ERROR = 2;


// message types 

// Update requests are sent by clients to server
struct INPUT_REQUEST {
	int input; 
};

struct INPUT_RESPONSE {
	char** gameBoard;
	bool _isAlive;
};

struct GAMESTATE_REQUEST {
	int state;
};

class Server {
//private
public:
	//void InitWinsock();
	//void RequestLogin();
	//void ResponseLogin();

	void RequestInput(INPUT_REQUEST, std::string);
	void ResponseInput();

	//void RequestEnd();
	//void ResponseEnd();
};

//void connectToServer(UPDATE_REQUEST, string);

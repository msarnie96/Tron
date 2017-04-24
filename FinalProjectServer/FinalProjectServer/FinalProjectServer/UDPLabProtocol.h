#ifndef _UDP_LAB_MSG_H_
#define _UDP_LAB_MSG_H_

#include <WinSock2.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#pragma pack(push) // save current packing 
#pragma pack(1)    // 1 byte alignment for structs in this file 

// The port number was chosen from dynamic or private ports from 
// https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml
const unsigned short PORT = 49263; 

// type field values
const char TYPE_ADD = 1;
const char TYPE_MULTIPLY = 2; 

// The server will respond with an error message if:
// - count is less than 2 or greater than 5
// - The type is not valid 
// - something goes wrong in the processing of data 
const char TYPE_ERROR = 3; 

// message types 

// requests are sent by clients 
struct REQUEST {
	char type; // add or multiply 
	short id;  // whatever the client wants - echoed by the server
	short count; // how many numbers are in the array 2 - 5 
	int numbers[5];
};

// responses are sent by servers 
struct RESPONSE {
	char type; // what the client sent, or error if count 
	short id; // what the client sent
	int result; // either the sum or product of count numbers
};

#pragma pack(pop) // back to default packing 

#endif



/*
** Quick and Dirty Keyboard Input Module
**
** Receive Input from a UDP Connection and set UAQs 
** in the CarMaker Simulation accordingly
**
** Functions
** ---------
** 
** Initialization
**
** UDP_Client ()
** UDP_Client_StartConnection ()
** UDP_Client_new() 
**
** Receive Data
**
** UDP_Client_GetCurrentInput()
**
** Shutdown
**
** UDP_Client_StopConnection()
** UDP_Client_delete()
*/

#include "UDP_Client.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Log.h>
#include <future>
#include <string>
#include <stdlib.h>
#include <exception>

#include <CarMaker.h>
#include <Car/Vehicle_Car.h>

#pragma comment (lib, "ws2_32.lib")

char currentInput;
int i = 0;
SOCKET in;
char buf[1024];
sockaddr_in client;
int clientLength;

u_long iMode = 1;

struct dataInput inputData;

UDP_Client::~UDP_Client() {

}


EXPORT_C int UDP_Client_StartConnection(UDP_Client*) {

	// Startup Winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0) {
		Log("Can't start winsock! %i", wsOk);
	}

	// Bind socket to ip address and port
	in = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000); // Convert from little to big endian

	Log("UDP_Client started on port 8051");

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
		Log("Can't bind socket! - %s \n", WSAGetLastError());
	}

	ZeroMemory(&client, sizeof(client));
	clientLength = sizeof(client);

	ZeroMemory(buf, 1024);

	return 0;
}

// access functions
EXPORT_C UDP_Client* UDP_Client_new(void)
{
	UDP_Client* InputSource = new UDP_Client();
	return InputSource;
}

EXPORT_C void UDP_Client_CurrentInput(UDP_Client*) {

	int bytesIn = ioctlsocket(in, FIONBIO, &iMode);
	recvfrom(in, buf, 1024, MSG_PARTIAL, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR) {
		Log("Error receiving from client - %s", WSAGetLastError());
		//continue;
	}

	char clientIp[256];
	ZeroMemory(clientIp, 256);

	//getnameinfo(AF_INET, &client.sin_addr, clientIp, 256);
	if (buf[0] != NULL) {
		Log("Message received");
	}

}


EXPORT_C void UDP_Client_GetCurrentUnityInput(UDP_Client*) {

	int bytesIn = ioctlsocket(in, FIONBIO, &iMode);
	recvfrom(in, buf, 1024, MSG_PARTIAL, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR) {
		Log("Error receiving from client - %s", WSAGetLastError());
		//continue;
	}

	char clientIp[256];
	ZeroMemory(clientIp, 256);

	//getnameinfo(AF_INET, &client.sin_addr, clientIp, 256);
	if (buf[0] != NULL) {
		//Log("Message received: %c, %c, %c, %c", buf[0], buf[1],buf[2],buf[3]);



		std::string delimiter = " ";
		std::vector<std::string> list;
		std::string s = std::string(buf);
		size_t pos = 0;
		std::string token;

		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			list.push_back(token);
			s.erase(0, pos + delimiter.length());
		}
		list.push_back(s);

		// Reformat received UDP Packet

		unityInput input;
		input.DVA_name = list[0].c_str();
		input.value = strtod(list[1].c_str(), NULL);
		input.duration = strtod(list[2].c_str(), NULL);

		// Log("%s, %f, %f", input.DVA_name, input.value, input.duration);

		if (strcmp(input.DVA_name, "0")) {
			DVA_WriteRequest(input.DVA_name, OWMode_Abs, input.duration, 0, 1000, input.value, "0");
			Log("%s, %f, %f", input.DVA_name, input.value, input.duration);
		}
	}


}

EXPORT_C void UDP_Client_StopConnection(UDP_Client*) {
	Log("Connection closed...");
	closesocket(in);
	WSACleanup();
}

EXPORT_C void UDP_Client_delete(UDP_Client*) {

}


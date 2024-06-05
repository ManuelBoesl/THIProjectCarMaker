/*
******************************************************************************
**
**
******************************************************************************
*/

#include "UDP_Sender.h"
#include "Data_Serializer.h"
#include "Data_Retriever.h"
#include "infofile.h"
#include "InfoUtils.h"

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Log.h>
#include <future>
#include <string>
#include "User.h"

#include <fstream>
#include "CarMakerData.pb.h"

#pragma comment (lib, "ws2_32.lib")
using namespace std;

tInfos * Inf_UDP = NULL;
tErrorMsg *err;

struct tNetworkData {
	string ipAdress;
	int port;
};

struct tNetworkData network_data;

int ret, rows = 0;

sockaddr_in server;
SOCKET out;

Data_Retriever retriever;

int current_Packet_number;

UDP_Sender::UDP_Sender() {
	currentInput = 'w';
}

UDP_Sender::~UDP_Sender() {

}

EXPORT_C int UDP_Sender_StartConnection(UDP_Sender*) {
	
	//Create infofile handle
	Inf_UDP = InfoNew();
	iRead2(&err, Inf_UDP, "Data/Config/UDPConfig", "IPAdress");

	//Read parameters
	network_data.ipAdress = iGetStr(Inf_UDP, "IPAdress");
	network_data.port = iGetInt(Inf_UDP, "Port");

	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0) {
		Log("Can't start winsock! \n");
	}

	
	server.sin_family = AF_INET;
	server.sin_port = htons(network_data.port);
	//PCSTR adress = MyData.ipAdress.c_str();

	inet_pton(AF_INET, network_data.ipAdress.c_str() , &server.sin_addr);

	out = socket(AF_INET, SOCK_DGRAM, 0);
	
	Log("Sender started with UDP Configuration: \n IP-Adress: %s \n Port: %i \n", 
		network_data.ipAdress, network_data.port);

	return 0;
}

char UDP_Sender::GetCurrentInput() {
	return currentInput;
}

// access functions
EXPORT_C UDP_Sender* UDP_Sender_new(void)
{
	UDP_Sender* InputSource = new UDP_Sender();
	return InputSource;
}

EXPORT_C void UDP_Sender_SendInput(UDP_Sender*) {

	// Retrieve the Data in serialized String form from the Data Retriever
	string serialized_data = retriever.Retrieve_Data();

	// Debug
	Log("%s \n", serialized_data.c_str());

	// Send the Data to Unity
	int sendOk = sendto(out, serialized_data.c_str(), (int)serialized_data.size() + 1, 0, (sockaddr*)&server, sizeof(server));
}

EXPORT_C void UDP_Sender_SendLastPaket(UDP_Sender*) {

	// Retrieve the Data in serialized String form from the Data Retriever
	string serialized_data = retriever.Retrieve_Data(2);

	// Debug
	Log("%s \n", serialized_data.c_str());

	// Send the Data to Unity
	int sendOk = sendto(out, serialized_data.c_str(), (int)serialized_data.size() + 1, 0, (sockaddr*)&server, sizeof(server));
}

EXPORT_C void UDP_Sender_SendFirstPaket(UDP_Sender*) {

	// Retrieve the Data in serialized String form from the Data Retriever
	string serialized_data = retriever.Retrieve_Data(1);

	// Debug
	Log("%s \n", serialized_data.c_str());

	// Send the Data to Unity
	int sendOk = sendto(out, serialized_data.c_str(), (int)serialized_data.size() + 1, 0, (sockaddr*)&server, sizeof(server));
}

EXPORT_C void UDP_Sender_SendProtoInput(UDP_Sender*) {
    CMData::CarMakerData data_to_send = retriever.Retrieve_CarMaker_Data(0);

    int sendOk = sendto(out, data_to_send.SerializeAsString().c_str(), (int)data_to_send.SerializeAsString().size() + 1, 0, (sockaddr*)&server, sizeof(server));
}

EXPORT_C void UDP_Sender_SendProtoLast(UDP_Sender*) {
    CMData::CarMakerData data_to_send = retriever.Retrieve_CarMaker_Data(2);

    int sendOk = sendto(out, data_to_send.SerializeAsString().c_str(), (int)data_to_send.SerializeAsString().size() + 1, 0, (sockaddr*)&server, sizeof(server));
}

EXPORT_C void UDP_Sender_SendProtoFirst(UDP_Sender*) {
    CMData::CarMakerData data_to_send = retriever.Retrieve_CarMaker_Data(1);

    int sendOk = sendto(out, data_to_send.SerializeAsString().c_str(), (int)data_to_send.SerializeAsString().size() + 1, 0, (sockaddr*)&server, sizeof(server));
}

EXPORT_C void UDP_Sender_StopConnection(UDP_Sender*) {
	Log("Connection closed... \n");
	closesocket(out);
	WSACleanup();
	current_Packet_number = 0;
}

EXPORT_C void UDP_Sender_delete(UDP_Sender*) {

}
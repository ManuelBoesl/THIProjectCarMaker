#pragma once

#include "UDP_Sender.h"
#include <string>

class Data_Retriever
{
	public:
			Data_Retriever();
			~Data_Retriever();
			std::string Retrieve_Data();
			std::string Retrieve_Data(int);
			Vhcl_Data Get_EgoVehicle_Data();
			Vhcl_Data Get_TrafficObject_Data(int);
			TrfLightCntrlData Get_TrfLight_Data(int, int);
};




#include "UDP_Sender.h"
#include <string>

#pragma once
class Data_Serializer
{
public:
	Data_Serializer();

	std::string Serialize_Vhcl_Data(Vhcl_Data);
	std::string Serialize_TrafficObj_Data(Vhcl_Data);
	std::string Serialize_TrfLight_Data(TrfLightCntrlData);
};


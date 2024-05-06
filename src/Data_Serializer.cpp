 /*
 ******************************************************************************
 ** This Module is used to 'Serialize' the Retrieved Vehicle and Traffic 
 ** Data from the Simulation into String form and prepare it for UDP
 ** Communication with Unity
 ******************************************************************************
 */
#include "Data_Serializer.h"
#include "UDP_Sender.h"
#include <string>

using namespace std;

Data_Serializer::Data_Serializer()
{

}

/*
** Retunrs the important Data from a Vehicle structure in string from 
** -> For UDP Communication with Unity
** @param: Vhcl_Data v = Vehicle Data Structure to be serialized into a string
** @return: Serialized Vehicle Data as a string; 
**			Values are seperated witha '_' in groups and a ';' between groups
*/
string Data_Serializer::Serialize_Vhcl_Data(Vhcl_Data v) {
	
	string serialized_Vhcl_Data;
	serialized_Vhcl_Data.append(to_string(v.x) + "_");
	serialized_Vhcl_Data.append(to_string(v.y) + "_");
	serialized_Vhcl_Data.append(to_string(v.z) + "_");
	serialized_Vhcl_Data.append(to_string(v.pitch) + "_");
	serialized_Vhcl_Data.append(to_string(v.roll) + "_");
	serialized_Vhcl_Data.append(to_string(v.yaw) + "_");
	serialized_Vhcl_Data.append(to_string(v.steeringWheelAngl)+ "_");
	serialized_Vhcl_Data.append(to_string(v.velocity));
	serialized_Vhcl_Data.append(";");
	
	return serialized_Vhcl_Data;
}

/*
** Returns the important Data from a Traffic Object and it's Vehicle structure in string form
** -> for UDP Communication with Unity 
** @param: Vhcl_Data v = Vehicle Data Structure to be serialized into a string
**			+ ToDo: Additional parameters (Name, Type, Color, ...)
** @return: Serialized Vehicle Data as a string;
**			Values are seperated witha '_' in groups and a ';' between groups
*/
string Data_Serializer::Serialize_TrafficObj_Data(Vhcl_Data v) {

	string serialized_TrafficObj_Data;

	serialized_TrafficObj_Data.append(to_string(v.x) + "_");
	serialized_TrafficObj_Data.append(to_string(v.y) + "_");
	serialized_TrafficObj_Data.append(to_string(v.z) + "_");
	serialized_TrafficObj_Data.append(to_string(v.pitch) + "_");
	serialized_TrafficObj_Data.append(to_string(v.roll) + "_");
	serialized_TrafficObj_Data.append(to_string(v.yaw) + "_");
	serialized_TrafficObj_Data.append(to_string(v.steeringWheelAngl) + "_");
	serialized_TrafficObj_Data.append(to_string(v.type));
	serialized_TrafficObj_Data.append(";");
	
	return serialized_TrafficObj_Data;
}

string Data_Serializer::Serialize_TrfLight_Data(TrfLightCntrlData d) {
	string serialized_TrfObj_Data;

	serialized_TrfObj_Data.append(to_string(d.ControlerID) + "_");
	serialized_TrfObj_Data.append(to_string(d.state) + "_");
	serialized_TrfObj_Data.append(to_string(d.Id) + ";");

	return serialized_TrfObj_Data;
}

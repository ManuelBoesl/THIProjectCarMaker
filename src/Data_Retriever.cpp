/*
******************************************************************************
** This Module is used to retrieve the data for all relevant objects, 
** that will need to be animated in Unity
******************************************************************************
*/

#include "Data_Retriever.h"
#include "Data_Serializer.h"
#include "DataDict.h"

#include <CarMaker.h>
#include <Vehicle.h>
#include <Traffic.h>
#include "UDP_Sender.h"
#include "infofile.h"
#include "InfoUtils.h"
#include "Log.h"
//#include "CarMakerData.pb.h"

#include <string>
#include <exception>
#include <map>
#include <iostream>
#include <fstream>

// Serializer used to write vehicle Data to a string -> for UDP Communication
Data_Serializer retrievedData_Serializer;

tDDictEntry *SteerAng;

Data_Retriever::Data_Retriever() {
}

Data_Retriever::~Data_Retriever() {

}

// This function is duplicate, remove later
/*
** Retrieve the Data from all relevant objects, that will need to be animated in Unity
** @param: None
** @return: String returnData = Data serialized into a string, ready for UDP communication
**			-> Can be passed on to the UDP_Sender Module in this form
*/
std::string Data_Retriever::Retrieve_Data() {

	std::string returnData;

	returnData.append(std::to_string(SimCore.Time) + ";");

	// Send Simulation Start Signal
	returnData.append(std::to_string(0) + ";");

	//Debug
	//Log("Data Retriever called \n");
	Vhcl_Data egoVhcl_Data;

	// Get data from the Ego vehicle
	egoVhcl_Data = Get_EgoVehicle_Data();
	returnData.append(retrievedData_Serializer.Serialize_Vhcl_Data(egoVhcl_Data));

	//Log("egoVhcl Data retrieved!");
	//Debug
	//Log("Number of Traffic Objects: %i \n", Traffic.nObjs);
	returnData.append("Traffic.nObj_" + std::to_string(Traffic.nObjs) + ";");


	//Get data from all Traffic Objects
	for (int i = 0; i < Traffic.nObjs; i++) {

		try {
			//Log("Traffic Object Number: %i was found! \n", i + 1 );
			if (Traffic_GetByTrfId(i) != 0) {

				//Debug
				/*Log("Object position: %d, %d, %d; Distance of T0%i: %d \n",
					Traffic_GetByTrfId(i)->t_0[0], Traffic_GetByTrfId(i)->t_0[0], Traffic_GetByTrfId(i)->t_0[0],
					i, Traffic_GetByTrfId(i)->Distance);
				*/
				Vhcl_Data currentTrfObjData = Get_TrafficObject_Data(i);
				returnData.append(retrievedData_Serializer.Serialize_Vhcl_Data(currentTrfObjData));
			}
			else {
				Log("TrafficObject couldn't be found -> UDP Data might be inconsistent!");
				// Create empty vehicle Data to keep the structure of the UDP Packet consistent
				Vhcl_Data* empty = new Vhcl_Data();
				returnData.append(retrievedData_Serializer.Serialize_TrafficObj_Data(*empty));
			}
		}
		catch (std::exception e) {
			Log("This did not work!");
		}
	}

    // This is all traffic light data, not needed for now
    // --------------------------------------------------------------------------------------------


//	// Append number of Traffic Lights present in the scenario
//	returnData.append("TrfLights.nObj_" + std::to_string(TrfLight.nObjs) + ";");
//
//	// Dictionary that contains TrfLight Data as values and geometric Object IDs as Keys
//	// -> The Traffic Light's geometric object contains the ObjID which is needed to match the Traffic light with it's
//	//	  Unity pendants
//	std::map<int, TrfLightCntrlData> TrfLightCntrlDataList;
//
//	// Get data from all Traffic Lights
//	// TrfLight contains a array of traffic light, which each containts another array with Traffic Light geometry objects
//	// -> Those geometry object arrays are similar in one junction controller but different between the junction controllers
//	// -> Here I use a Dictionary to check if the Traffic light geometric object has already been added to the dictionary
//	// -> There is no usable documentation by IPG on how the traffic light system in CM works, so this is guesswork
//	for (int y = 0; y < TrfLight.nObjs; y++) {
//		for (int z = 0; z < sizeof(TrfLight.Objs[y].TLList); z++) {
//			try {
//				TrfLightCntrlData trf_light = Get_TrfLight_Data(y, z);
//
//				if (TrfLightCntrlDataList.find(TrfLight.Objs[y].TLList[z].ObjId) == TrfLightCntrlDataList.end()) {
//					// Object IDs of the geometric objects are sometimes not assigned and can contain 0 or
//					// non-integer values for some reason
//					if (TrfLight.Objs[y].TLList[z].ObjId > 0 && TrfLight.Objs[y].TLList[z].ObjId < 1000) {
//						TrfLightCntrlDataList[TrfLight.Objs[y].TLList[z].ObjId] = trf_light;
//					}
//				}
//			}
//			catch (std::exception e) {
//				Log("Traffic light couldn't be read");
//			}
//			// Debug
//			// Log("Current entry: %i, %i \n", TrfLight.Objs[6].TLList[y].ObjId, y);
//		}
//	}
//
//	// Serialize Traffic Light Data and append it to the return data
//	for (auto& x : TrfLightCntrlDataList) {
//		returnData.append(retrievedData_Serializer.Serialize_TrfLight_Data(x.second));
//	}

	return returnData;
}

std::string Data_Retriever::Retrieve_Data(int signalToSend) {

	std::string returnData;

	returnData.append(std::to_string(SimCore.Time) + ";");

	// Send Simulation Start Signal
	returnData.append(std::to_string(signalToSend) + ";");

	//Debug
	//Log("Data Retriever called \n");
	Vhcl_Data egoVhcl_Data;

	// Get data from the Ego vehicle
	egoVhcl_Data = Get_EgoVehicle_Data();
	returnData.append(retrievedData_Serializer.Serialize_Vhcl_Data(egoVhcl_Data));

	//Log("egoVhcl Data retrieved!");
	//Debug
	//Log("Number of Traffic Objects: %i \n", Traffic.nObjs);
	returnData.append("Traffic.nObj_" + std::to_string(Traffic.nObjs) + ";");


	//Get data from all Traffic Objects
	for (int i = 0; i < Traffic.nObjs; i++) {

		try {
			//Log("Traffic Object Number: %i was found! \n", i + 1 );
			if (Traffic_GetByTrfId(i) != 0) {

				//Debug
				/*Log("Object position: %d, %d, %d; Distance of T0%i: %d \n",
					Traffic_GetByTrfId(i)->t_0[0], Traffic_GetByTrfId(i)->t_0[0], Traffic_GetByTrfId(i)->t_0[0],
					i, Traffic_GetByTrfId(i)->Distance);
				*/
				Vhcl_Data currentTrfObjData = Get_TrafficObject_Data(i);
				returnData.append(retrievedData_Serializer.Serialize_Vhcl_Data(currentTrfObjData));
			}
			else {
				Log("TrafficObject couldn't be found -> UDP Data might be inconsistent!");
				// Create empty vehicle Data to keep the structure of the UDP Packet consistent
				Vhcl_Data* empty = new Vhcl_Data();
				returnData.append(retrievedData_Serializer.Serialize_TrafficObj_Data(*empty));
			}
		}
		catch (std::exception e) {
			Log("This did not work!");
		}
	}



//	// Append number of Traffic Lights present in the scenario
//	returnData.append("TrfLights.nObj_" + std::to_string(TrfLight.nObjs) + ";");
//
//	// Dictionary that contains TrfLight Data as values and geometric Object IDs as Keys
//	// -> The Traffic Light's geometric object contains the ObjID which is needed to match the Traffic light with it's
//	//	  Unity pendants
//	std::map<int, TrfLightCntrlData> TrfLightCntrlDataList;
//
//	// Get data from all Traffic Lights
//	// TrfLight contains a array of traffic light, which each containts another array with Traffic Light geometry objects
//	// -> Those geometry object arrays are similar in one junction controller but different between the junction controllers
//	// -> Here I use a Dictionary to check if the Traffic light geometric object has already been added to the dictionary
//	// -> There is no usable documentation by IPG on how the traffic light system in CM works, so this is guesswork
//	for (int y = 0; y < TrfLight.nObjs; y++) {
//		for (int z = 0; z < sizeof(TrfLight.Objs[y].TLList); z++) {
//			try {
//				TrfLightCntrlData trf_light = Get_TrfLight_Data(y, z);
//
//				if (TrfLightCntrlDataList.find(TrfLight.Objs[y].TLList[z].ObjId) == TrfLightCntrlDataList.end()) {
//					// Object IDs of the geometric objects are sometimes not assigned and can contain 0 or
//					// non-integer values for some reason
//					if (TrfLight.Objs[y].TLList[z].ObjId > 0 && TrfLight.Objs[y].TLList[z].ObjId < 1000) {
//						TrfLightCntrlDataList[TrfLight.Objs[y].TLList[z].ObjId] = trf_light;
//					}
//				}
//			}
//			catch (std::exception e) {
//				Log("Traffic light couldn't be read");
//			}
//			// Debug
//			// Log("Current entry: %i, %i \n", TrfLight.Objs[6].TLList[y].ObjId, y);
//		}
//	}
//
//	// Serialize Traffic Light Data and append it to the return data
//	for (auto& x : TrfLightCntrlDataList) {
//		returnData.append(retrievedData_Serializer.Serialize_TrfLight_Data(x.second));
//	}

	return returnData;
}

/*
** Returns the Data from the Ego-Vehicle
** @param: None
** @return: Vhcl_Data egoVhcl_Data = Vehicle Data structure containing the data from the Ego Vehicle
*/
Vhcl_Data Data_Retriever::Get_EgoVehicle_Data() {

	Vhcl_Data egoVhcl_Data;

	// Get data for the Ego Vehicle
	egoVhcl_Data.x = Vehicle.Fr1A.t_0[0];
	egoVhcl_Data.y = Vehicle.Fr1A.t_0[1];
	egoVhcl_Data.z = Vehicle.Fr1A.t_0[2];
	egoVhcl_Data.pitch = Vehicle.Pitch;
	egoVhcl_Data.roll = Vehicle.Roll;
	egoVhcl_Data.yaw = Vehicle.Yaw;
	egoVhcl_Data.steeringWheelAngl = Vehicle.Steering.Ang;
	egoVhcl_Data.velocity = Vehicle.v;
    egoVhcl_Data.id = 424711; // This might have to be changed

	return egoVhcl_Data;
}

/*	
** Returns the Data for a specific Traffic-Object
** @param: int trafficObjId = ID from the Traffic Objects which the data should be retrieved
** @return: Vhcl_Data trafficObject_Data = Vehicle Data structure containing the data from the Traffic Object
*/
Vhcl_Data Data_Retriever::Get_TrafficObject_Data(int trafficObjId) {
	
	Vhcl_Data trafficObject_Data;

	// Get current Traffic Object by ID
	tTrafficObj trfObj;
	trfObj = *Traffic_GetByTrfId(trafficObjId);

	trafficObject_Data.x = trfObj.t_0[0];
	trafficObject_Data.y = trfObj.t_0[1];
	trafficObject_Data.z = trfObj.t_0[2];
	trafficObject_Data.pitch = trfObj.r_zyx[0];
	trafficObject_Data.roll = trfObj.r_zyx[1];
	trafficObject_Data.yaw = trfObj.r_zyx[2];
	trafficObject_Data.type = trfObj.Cfg.RCSClass;
    trafficObject_Data.id = trafficObjId;

	// Steering Wheel Angle in Traffic Objects represent the Rotation of the wheel
	// -> Because the rotation and position variables for the wheels of a traffic-
	// object can't be accessed
	// Traffic.<trafficObjectId>.SteerAng is also private and can't be accessed directly
	// -> Because of this a DDictEntry is needed to read the UAQ
	try {

		// Parse the correct Identifier for the Traffic Object's Steering Wheel Angle DDictEntry
		// form the trafficObjId 
		std::string DVA_Name;
		if(trafficObjId <10 )
			DVA_Name = "Traffic.T0" + std::to_string(trafficObjId) + ".SteerAng";
		else
			DVA_Name = "Traffic.T" + std::to_string(trafficObjId) + ".SteerAng";
		
		// Get the DDictEntry for the Traffic Object's Steering Angle
		SteerAng = DDictGetEntry(DVA_Name.c_str());
		// Retrieve the valiue from the Steering Angle Data Dictionary Entry
		trafficObject_Data.steeringWheelAngl = SteerAng->GetFunc(SteerAng->Var);
		//Debug
		//Log(" Traffic object steering angle is: %d \n", SteerAng->GetFunc(SteerAng->Var));

	}
	catch (std::exception e) {
		Log("Did not find the Data Dictionary Entry for Traffic Object's Steering Wheel Angle UAQ! \n");
		SteerAng = 0;
	}



	return trafficObject_Data;
}

//CMData::CarMakerData Retrieve_CarMaker_Data(int trafficObjId) {
//    CMData::CarMakerData carMakerData;
//
//    carMakerData.set_timestamp(std::to_string(SimCore.Time));
//    carMakerData.set_stimulationsignal(trafficObjId);
//    // Send Simulation Start Signal
//
//    //Debug
//    //Log("Data Retriever called \n");
//    Vhcl_Data egoVhcl_Data;
//
//    // Get data from the Ego vehicle
//    egoVhcl_Data = Get_EgoVehicle_Data();
//
//    auto egoVhclDataGroup = carMakerData.add_carmakerdatagroup();
//    egoVhclDataGroup->set_id(egoVhcl_Data.id);
//    egoVhclDataGroup->set_type(egoVhcl_Data.type);
//
//    egoVhclDataGroup->set_steeringangle((float) egoVhcl_Data.steeringWheelAngl);
//
//    egoVhclDataGroup->position().set_posx((float) egoVhcl_Data.x);
//    egoVhclDataGroup->position().set_posy((float) egoVhcl_Data.y);
//    egoVhclDataGroup->position().set_posz((float) egoVhcl_Data.z);
//
//    egoVhclDataGroup->rotation().set_rotx((float) egoVhcl_Data.pitch);
//    egoVhclDataGroup->rotation().set_roty((float) egoVhcl_Data.roll);
//    egoVhclDataGroup->rotation().set_rotz((float) egoVhcl_Data.yaw);
//
//    //Log("egoVhcl Data retrieved!");
//    //Debug
//    //Log("Number of Traffic Objects: %i \n", Traffic.nObjs);
//
//
//    //Get data from all Traffic Objects
//    for (int i = 0; i < Traffic.nObjs; i++) {
//
//        try {
//            //Log("Traffic Object Number: %i was found! \n", i + 1 );
//            if (Traffic_GetByTrfId(i) != 0) {
//
//                //Debug
//                /*Log("Object position: %d, %d, %d; Distance of T0%i: %d \n",
//                    Traffic_GetByTrfId(i)->t_0[0], Traffic_GetByTrfId(i)->t_0[0], Traffic_GetByTrfId(i)->t_0[0],
//                    i, Traffic_GetByTrfId(i)->Distance);
//                */
//                Vhcl_Data currentTrfObjData = Get_TrafficObject_Data(i);
//
//                auto currentTrfObjDataGroup = carMakerData.add_carmakerdatagroup();
//
//                currentTrfObjDataGroup->set_id(egoVhcl_Data.id);
//                currentTrfObjDataGroup->set_type(egoVhcl_Data.type);
//
//                currentTrfObjDataGroup->set_steeringangle((float) egoVhcl_Data.steeringWheelAngl);
//
//                currentTrfObjDataGroup->position().set_posx((float) egoVhcl_Data.x);
//                currentTrfObjDataGroup->position().set_posy((float) egoVhcl_Data.y);
//                currentTrfObjDataGroup->position().set_posz((float) egoVhcl_Data.z);
//
//                currentTrfObjDataGroup->rotation().set_rotx((float) egoVhcl_Data.pitch);
//                currentTrfObjDataGroup->rotation().set_roty((float) egoVhcl_Data.roll);
//                currentTrfObjDataGroup->rotation().set_rotz((float) egoVhcl_Data.yaw);
//
//            }
//            else {
//                // Check if the if-else block is really needed
//                Log("TrafficObject couldn't be found -> UDP Data might be inconsistent!");
//                // Create empty vehicle Data to keep the structure of the UDP Packet consistent
//                Vhcl_Data empty;
//
//                empty->set_id(egoVhcl_Data.id);
//                empty->set_type(egoVhcl_Data.type);
//
//                empty->set_steeringangle((float) egoVhcl_Data.steeringWheelAngl);
//
//                empty->position().set_posx((float) egoVhcl_Data.x);
//                empty->position().set_posy((float) egoVhcl_Data.y);
//                empty->position().set_posz((float) egoVhcl_Data.z);
//
//                empty->rotation().set_rotx((float) egoVhcl_Data.pitch);
//                empty->rotation().set_roty((float) egoVhcl_Data.roll);
//                empty->rotation().set_rotz((float) egoVhcl_Data.yaw);
//
//            }
//        }
//        catch (std::exception e) {
//            Log("This did not work!");
//        }
//    }
//
//// This whole stuff is not needed for now
////    // Dictionary that contains TrfLight Data as values and geometric Object IDs as Keys
////    // -> The Traffic Light's geometric object contains the ObjID which is needed to match the Traffic light with it's
////    //	  Unity pendants
////    std::map<int, TrfLightCntrlData> TrfLightCntrlDataList;
////
////    // Get data from all Traffic Lights
////    // TrfLight contains a array of traffic light, which each containts another array with Traffic Light geometry objects
////    // -> Those geometry object arrays are similar in one junction controller but different between the junction controllers
////    // -> Here I use a Dictionary to check if the Traffic light geometric object has already been added to the dictionary
////    // -> There is no usable documentation by IPG on how the traffic light system in CM works, so this is guesswork
////    for (int y = 0; y < TrfLight.nObjs; y++) {
////        for (int z = 0; z < sizeof(TrfLight.Objs[y].TLList); z++) {
////            try {
////                TrfLightCntrlData trf_light = Get_TrfLight_Data(y, z);
////
////                if (TrfLightCntrlDataList.find(TrfLight.Objs[y].TLList[z].ObjId) == TrfLightCntrlDataList.end()) {
////                    // Object IDs of the geometric objects are sometimes not assigned and can contain 0 or
////                    // non-integer values for some reason
////                    if (TrfLight.Objs[y].TLList[z].ObjId > 0 && TrfLight.Objs[y].TLList[z].ObjId < 1000) {
////                        TrfLightCntrlDataList[TrfLight.Objs[y].TLList[z].ObjId] = trf_light;
////                    }
////                }
////            }
////            catch (std::exception e) {
////                Log("Traffic light couldn't be read");
////            }
////            // Debug
////            // Log("Current entry: %i, %i \n", TrfLight.Objs[6].TLList[y].ObjId, y);
////        }
////    }
////
////    // Serialize Traffic Light Data and append it to the return data
////    for (auto& x : TrfLightCntrlDataList) {
////        returnData.append(retrievedData_Serializer.Serialize_TrfLight_Data(x.second));
////    }
//
//    return carMakerData;
//}

TrfLightCntrlData Data_Retriever::Get_TrfLight_Data(int TrfObj_ID, int TLList_ID) {
	TrfLightCntrlData returnData;
	
	returnData.ControlerID = TrfLight.Objs[TrfObj_ID].ObjId;
	returnData.state = TrfLight.Objs[TrfObj_ID].State;
	
	// Log("TrfLightObjID: %i , %i\n", TrfLight.Objs[TrfObj_ID].TLList[TrfObj_ID].ObjId, TrfObj_ID);
	returnData.Id = TrfLight.Objs[TrfObj_ID].TLList[TLList_ID].ObjId;

	return returnData;
}

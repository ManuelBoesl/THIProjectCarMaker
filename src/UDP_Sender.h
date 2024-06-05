#include "User.h"

#pragma once

typedef struct Vhcl_Data {
	double x, y, z;
	double pitch, roll, yaw;
	double steeringWheelAngl;
	double velocity;
	int type;
    int id;
}Vhcl_Data;

typedef struct TrfLightCntrlData {
	int ControlerID;
	int state;
	int Id;
}TrfcLightCntrlData	;

typedef struct TrfLightPosData {
	double pos[3];
	double rot[3];
	int id;
}TrfLightPosData;

#ifdef __cplusplus // only actually define the class if this is C++
class UDP_Sender
{
public:
	UDP_Sender();
	~UDP_Sender();
	char GetCurrentInput();
	
private:
	char currentInput;
};

#else

// C doesn't know about classes, just say it's a struct
typedef struct UDP_Sender UDP_Sender;

#endif

// access functions
#ifdef __cplusplus
#define EXPORT_C extern "C"
#else
#define EXPORT_C
#endif

EXPORT_C UDP_Sender* UDP_Sender_new(void);
EXPORT_C void UDP_Sender_delete(UDP_Sender*);
EXPORT_C void UDP_Sender_SendInput(UDP_Sender*);
EXPORT_C void UDP_Sender_SendLastPaket(UDP_Sender*);
EXPORT_C void UDP_Sender_SendFirstPaket(UDP_Sender*);
EXPORT_C int UDP_Sender_StartConnection(UDP_Sender*);
EXPORT_C void UDP_Sender_StopConnection(UDP_Sender*);

//EXPORT_C void UDP_Sender_SendProtoInput(UDP_Sender*);
//EXPORT_C void UDP_Sender_SendProtoLast(UDP_Sender*);
//EXPORT_C void UDP_Sender_SendProtoFirst(UDP_Sender*);
#pragma once


typedef struct dataInput {
	double gas;
	double brake;
	double steerAngl;
	double autoDrive;
} dataInput;

typedef struct unityInput {
	const char* DVA_name;
	double value;
	double duration;
} unityInput;


#ifdef __cplusplus // only actually define the class if this is C++
class UDP_Client
{
public:
	~UDP_Client();
private:
	char currentInput;
	dataInput ReformatInputData(char [1024]);
};

#else

// C doesn't know about classes, just say it's a struct
typedef struct UDP_Client UDP_Client;

#endif

// access functions
#ifdef __cplusplus
#define EXPORT_C extern "C"
#else
#define EXPORT_C
#endif

EXPORT_C UDP_Client* UDP_Client_new(void);
EXPORT_C void UDP_Client_delete(UDP_Client*);
EXPORT_C void UDP_Client_CurrentInput(UDP_Client*);
EXPORT_C void UDP_Client_GetCurrentUnityInput(UDP_Client*);
EXPORT_C int UDP_Client_StartConnection(UDP_Client*);
EXPORT_C void UDP_Client_StopConnection(UDP_Client*);
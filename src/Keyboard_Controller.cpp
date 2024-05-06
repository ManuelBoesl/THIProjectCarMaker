#include "Keyboard_Controller.h"
#include "UDP_Client.h"

#include <CarMaker.h>
#include <Car/Vehicle_Car.h>


struct dataInput input;
double sum = 0;
double oldSum = 0;

UDP_Client* client;

Keyboard_Controller::Keyboard_Controller() {

}

EXPORT_C Keyboard_Controller* Keyboard_Controller_new(void) {
	Keyboard_Controller* controller = new Keyboard_Controller();	
	client = UDP_Client_new();
	UDP_Client_StartConnection(client);
	return controller;
};

EXPORT_C void Keyboard_Controller_TranslateEgoVhcl(Keyboard_Controller*) {
	input = UDP_Client_GetCurrentInput(client);

	// Generate Checksum -> Will be zero if no new Message was received
	sum = input.gas + input.brake + input.steerAngl;

	// Check if current driving mode is manual or automated
	if (input.autoDrive != 0) {
		// set driving mode to manual
		DVA_WriteRequest("Driver.Long.passive", OWMode_Abs, 100, 10000.0, 10000, 1, "1");
		DVA_WriteRequest("Driver.Lat.passive", OWMode_Abs, 100, 10000.0, 10000, 1, "1");
		DVA_WriteRequest("Driver.Gas", OWMode_Abs, 20, 10000.0, 10000, 1, "1");
		DVA_WriteRequest("Driver.Brake", OWMode_Abs, 20, 10000.0, 10000, 1, "1");
	}
	else {
		// set driving mode to automated
		DVA_WriteRequest("Driver.Long.passive", OWMode_Abs, 100, 10000.0, 10000, 0, "1");
		DVA_WriteRequest("Driver.Lat.passive", OWMode_Abs, 100, 10000.0, 10000, 0, "1");
	}

	// Return if no new message was received
	if (sum == 0 && oldSum == 0) {
		return;
	}

	// Send Direct Variable Access Write Requests according to the input
	DVA_WriteRequest("DM.Steer.Ang", OWMode_Abs, 20, 10000.0, 10000, input.steerAngl, "0");
	DVA_WriteRequest("VC.Brake", OWMode_Abs, 20, 10000.0, 10000, input.brake, "0");
	DVA_WriteRequest("VC.Gas", OWMode_Abs, 20, 10000.0, 10000, input.gas, "0");

	oldSum = sum;
}
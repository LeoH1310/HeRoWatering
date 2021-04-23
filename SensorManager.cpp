// 
// 
// 

#include "SensorManager.h"
#include "EmailMangager.h"

bool checkWaterLevel() {

	// DEBUG
	bool sendStatus = sendEmail("AquaHeRo: Wasserstand kritisch!", "Der Wasserstand ist zu niedrig, um AquaHeRo zu starten.", "hesseleo1310@gmail.com");
}


bool checkMoistureLevel() {

	Moisture moistureErdbeeren = sensorErdbeeren->getValue();
	Moisture moistureTomaten = sensorTomaten->getValue();

	bool watering;
	if (moistureErdbeeren.priority + moistureTomaten.priority >= 0) {
		// Bewässerung notwendig
		watering = true;
	}
	else {
		// Keine Bewässerung notwendig
		watering = false;
	}

	//ToDo: logging

	return watering;
}



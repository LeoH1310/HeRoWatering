// 
// 
// 

#include "SensorManager.h"

bool checkWaterLevel() {
	return (digitalRead(waterLevelPin));
}

bool checkForWatering(Moisture moistureErdbeeren, Moisture moistureTomaten) {

	if (moistureErdbeeren.priority + moistureTomaten.priority >= 0) {
		// Bew�sserung notwendig
		return true;
	}
	else {
		// Keine Bew�sserung notwendig
		return false;
	}
}



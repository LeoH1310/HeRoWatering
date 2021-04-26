// 
// 
// 

#include "SensorManager.h"

bool checkWaterLevel() {
	return (digitalRead(waterLevelPin));
}

bool checkForWatering(Moisture moistureErdbeeren, Moisture moistureTomaten) {

	if (moistureErdbeeren.priority + moistureTomaten.priority >= 0) {
		// Bewässerung notwendig
		return true;
	}
	else {
		// Keine Bewässerung notwendig
		return false;
	}
}



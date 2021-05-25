// 
// 
// 

#include "SensorManager.h"

bool checkWaterLevel() {
	return (digitalRead(waterLevelPin));
}

int checkForWatering(Moisture moistureErdbeeren, Moisture moistureTomaten) {

	if (moistureErdbeeren.priority + moistureTomaten.priority >= 0) {
		// Bew�sserung notwendig
		// Priorit�t bestimmt Wassermenge
		return std::max(moistureErdbeeren.priority, moistureTomaten.priority);
	}
	else {
		// Keine Bew�sserung notwendig
		return 0;
	}
}



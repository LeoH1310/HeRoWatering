// 
// 
// 

#include "SensorManager.h"

bool checkWaterLevel() {
	return (digitalRead(waterLevelPin));
}

int checkForWatering(Moisture moistureErdbeeren, Moisture moistureTomaten) {

	if (moistureErdbeeren.priority + moistureTomaten.priority >= 0) {
		// Bewässerung notwendig
		// Priorität bestimmt Wassermenge
		return std::max(moistureErdbeeren.priority, moistureTomaten.priority);
	}
	else {
		// Keine Bewässerung notwendig
		return 0;
	}
}



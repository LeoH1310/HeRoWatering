// 
// 
// 

#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(int pin, int air, int water) {
	MoistureSensor::sonsorPin = pin;
	MoistureSensor::sensorAir = air;
	MoistureSensor::sensorWater = water;
	MoistureSensor::sensorIntervals = (air - water) / 5;
	MoistureSensor::currentMoisture = new Moisture();
}

Moisture MoistureSensor::getValue() {
	int rawValue = 0;
	for (int i = 0; i < 5; i++) {
		rawValue = analogRead(MoistureSensor::sonsorPin);
		delay(100);
	}
	rawValue /= 5;
	currentMoisture->rawValue = rawValue;
	currentMoisture->level = MoistureSensor::getMoistureLevel(rawValue);
	
	switch (currentMoisture->level) {
	case MoistureLevel::veryWet:	currentMoisture->priority = -10;
	case MoistureLevel::wet:		currentMoisture->priority = -5;
	case MoistureLevel::damp:		currentMoisture->priority = 0;
	case MoistureLevel::dry:		currentMoisture->priority = 5;
	case MoistureLevel::veryDry:	currentMoisture->priority = 11;
	}

	return *this->currentMoisture;
}

MoistureLevel MoistureSensor::getMoistureLevel(int raw) {
	if (raw < (MoistureSensor::sensorWater + MoistureSensor::sensorIntervals)) {
		return MoistureLevel::veryWet;
	}
	else if (raw > (MoistureSensor::sensorWater + MoistureSensor::sensorIntervals) && raw < (MoistureSensor::sensorWater + 2*MoistureSensor::sensorIntervals)) {
		return MoistureLevel::wet;
	}
	else if (raw > (MoistureSensor::sensorWater + 2 * MoistureSensor::sensorIntervals) && raw < (MoistureSensor::sensorWater + 3 * MoistureSensor::sensorIntervals)) {
		return MoistureLevel::damp;
	}
	else if (raw > (MoistureSensor::sensorWater + 3 * MoistureSensor::sensorIntervals) && raw < (MoistureSensor::sensorWater + 4 * MoistureSensor::sensorIntervals)) {
		return MoistureLevel::dry;
	}
	else {
		return MoistureLevel::veryDry;
	}
}

char* Moisture::getName() {
	char* s("unknown");
	switch (this->level)    {
	case 0: { s = "veryWet"; } break;
	case 1: { s = "wet"; } break;
	case 2: { s = "damp"; } break;
	case 3: { s = "dry"; } break;
	case 4: { s = "veryDry"; } break;
	}
	return s;
}


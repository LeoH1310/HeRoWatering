// 
// 
// 

#define RUNS 25

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
	for (int i = 0; i < RUNS; i++) {
		rawValue += analogRead(MoistureSensor::sonsorPin);
		delay(100);
	}
	rawValue /= RUNS;
	currentMoisture->rawValue = rawValue;
	currentMoisture->level = MoistureSensor::getMoistureLevel(rawValue);
	
	switch (currentMoisture->level) {
		case MoistureLevel::water:		currentMoisture->priority = 0;		break;
		case MoistureLevel::veryWet:	currentMoisture->priority = -1;		break;
		case MoistureLevel::wet:		currentMoisture->priority = -1;		break;
		case MoistureLevel::damp:		currentMoisture->priority = 1;		break;
		case MoistureLevel::dry:		currentMoisture->priority = 2;		break;
		case MoistureLevel::veryDry:	currentMoisture->priority = 3;		break;
		case MoistureLevel::air:		currentMoisture->priority = 3;		break;
	}

	return *this->currentMoisture;
}

MoistureLevel MoistureSensor::getMoistureLevel(int raw) {
	if (raw < MoistureSensor::sensorWater) {
		return MoistureLevel::water;
	}
	else if (raw >= MoistureSensor::sensorWater && raw < (MoistureSensor::sensorWater + MoistureSensor::sensorIntervals)) {
		return MoistureLevel::veryWet;
	}
	else if (raw >= (MoistureSensor::sensorWater + MoistureSensor::sensorIntervals) && raw < (MoistureSensor::sensorWater + 2*MoistureSensor::sensorIntervals)) {
		return MoistureLevel::wet;
	}
	else if (raw >= (MoistureSensor::sensorWater + 2 * MoistureSensor::sensorIntervals) && raw < (MoistureSensor::sensorWater + 3 * MoistureSensor::sensorIntervals)) {
		return MoistureLevel::damp;
	}
	else if (raw >= (MoistureSensor::sensorWater + 3 * MoistureSensor::sensorIntervals) && raw < (MoistureSensor::sensorWater + 4 * MoistureSensor::sensorIntervals)) {
		return MoistureLevel::dry;
	}
	else if (raw >= (MoistureSensor::sensorWater + 4 * MoistureSensor::sensorIntervals) && raw < MoistureSensor::sensorAir) {
		return MoistureLevel::veryDry;
	}
	else {
		return MoistureLevel::air;
	}
}

char* Moisture::getName() {
	char* s("unknown");
	switch (this->level)    {
		case MoistureLevel::water:		{ s = "water"; }	break;
		case MoistureLevel::veryWet:	{ s = "veryWet"; }	break;
		case MoistureLevel::wet:		{ s = "wet"; }		break;
		case MoistureLevel::damp:		{ s = "damp"; }		break;
		case MoistureLevel::dry:		{ s = "dry"; }		break;
		case MoistureLevel::veryDry:	{ s = "veryDry"; }	break;
		case MoistureLevel::air:		{ s = "air"; }		break;
	}
	return s;
}


// 
// 
// 

#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(int pin, int air, int water) {
	MoistureSensor::sonsorPin = pin;
	MoistureSensor::sensorAir = air;
	MoistureSensor::sensorWater = water;
	MoistureSensor::sensorIntervals = (air - water) / 5;
}

Moisture MoistureSensor::getValue() {
	int rawValue = 0;
	for (int i = 0; i < 5; i++) {
		rawValue = analogRead(MoistureSensor::sonsorPin);
		delay(100);
	}
	rawValue /= 5;
	Moisture result {rawValue, MoistureSensor::getMoistureLevel(rawValue)};
	
	switch (result.level) {
	case MoistureLevel::veryWet:	result.priority = -10;
	case MoistureLevel::wet:		result.priority = -5;
	case MoistureLevel::damp:		result.priority = 0;
	case MoistureLevel::dry:		result.priority = 5;
	case MoistureLevel::veryDry:	result.priority = 11;
	}

	return result;
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
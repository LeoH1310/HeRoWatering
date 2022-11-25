// MoistureSensor.h

#pragma once 

#include "arduino.h"

enum MoistureLevel {water, veryWet, wet, damp, dry, veryDry, air};

class Moisture {
public:
	int rawValue;
	int priority;
	MoistureLevel level;

	char* getName();
};

class MoistureSensor
{
private:
	int sonsorPin;
	int sensorAir;
	int sensorWater;
	int sensorIntervals;
	Moisture * currentMoisture;

	MoistureLevel getMoistureLevel(int raw);

public:
	 MoistureSensor(int pin, int air, int water);

	 Moisture getValue();
};


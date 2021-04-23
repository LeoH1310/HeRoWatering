// MoistureSensor.h

#pragma once 

#include "arduino.h"

#include "Credentials.h"

enum MoistureLevel {veryWet, wet, damp, dry, veryDry};

struct Moisture {
	int rawValue;
	MoistureLevel level;
	int priority;
};

class MoistureSensor
{
private:
	int sonsorPin;
	int sensorAir;
	int sensorWater;
	int sensorIntervals;
	MoistureLevel getMoistureLevel(int raw);

public:
	 MoistureSensor(int pin, int air, int water);
	 Moisture getValue();
};


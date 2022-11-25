// SensorManager.h

#pragma once

#include "arduino.h"

#include <algorithm> 

#include "MoistureSensor.h"

extern const int sensor1Pin;
extern const int sensor1Air;
extern const int sensor1Water;
extern const int sensor2Pin;
extern const int sensor2Air;
extern const int sensor2Water;
extern const int waterLevelPin;

bool checkWaterLevel();
int checkForWatering(Moisture moistureErdbeeren, Moisture moistureTomaten);
// SensorManager.h

#pragma once

#include "arduino.h"

#include "Credentials.h"
#include "MoistureSensor.h"

extern const int sensor1Pin;
extern const int sensor1Air;
extern const int sensor1Water;
extern const int sensor2Pin;
extern const int sensor2Air;
extern const int sensor2Water;

MoistureSensor * sensorErdbeeren = new MoistureSensor(sensor1Pin, sensor1Air, sensor1Water);
MoistureSensor * sensorTomaten = new MoistureSensor(sensor2Pin, sensor2Air, sensor2Water);

bool checkWaterLevel();
bool checkMoistureLevel();


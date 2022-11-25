// WeatherAPI.h

#pragma once

#define ARDUINOJSON_USE_LONG_LONG 1

#include "arduino.h"
#include "time.h"
#include <ArduinoJson.h> 
#include <ArduinoJson.hpp>
#include <SPI.h> 
#include <WiFiNINA.h>

class Weather {
public:
	time_t date;
	time_t sunrise;
	time_t sunset;
	float temp;
	float rainProbability;
	float rainAmount;

	void update(WiFiSSLClient client);
};

extern const String apiKey;
extern const String location;
extern const char* server;

ARDUINOJSON_NAMESPACE::StaticJsonDocument<5000>* getWeather(WiFiClient client);

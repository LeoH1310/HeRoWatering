#pragma once

#include "arduino.h"

using namespace std;

extern const char* HOST = "AquaHeRo";
extern const char* SSID = "Relax_2.4";
extern const char* PASS = "leosabi782013";

extern const char* MAIL = "IoTAquaHeRo@gmail.com";
extern const char* MAILPASS = "p3x742a1b2";

extern const double latitude = 49.02131768569845;
extern const double longitude = 12.10671662706796;
extern const float timezone = +2.0;

extern const int offsetMin = 30;

extern const IPAddress SQL_IP(192, 168, 100, 10);
extern const uint16_t SQL_PORT = 3306;
extern const char* SQL_DB = "AquaHeRo";
extern const char* SQL_TABLE = "EventLog";
extern char* SQL_USER = "HeRo";
extern char* SQL_PASS = "LeoSabi_782013";
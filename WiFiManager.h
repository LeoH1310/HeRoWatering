// DriverWiFi.h

#pragma once

#include "arduino.h"

// Libraries for Wifi
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

extern const char* HOST;
extern const char* SSID;
extern const char* PASS;

void connectWiFi();


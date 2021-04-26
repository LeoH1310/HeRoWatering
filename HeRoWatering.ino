/*
 Name:		HeRoWatering.ino
 Created:	14.04.2021 19:25:55
 Author:	Leonhard Hesse
*/

#include "Credentials.h"
#include "WiFiManager.h"
#include "RtcAlarmManager.h"
#include "EmailMangager.h"
#include "SensorManager.h"
#include "DatabaseManager.h"
#include "MoistureSensor.h"

extern const int waterLevelPin;

RTCZero rtc;
WiFiClient client;
WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "de.pool.ntp.org", 0, 0);
EMailSender emailSender(MAIL, MAILPASS);

// the setup function runs once when you press reset or power the board
void setup() {
	// Debugging
	Serial.begin(115200);

	// cofig waterlevel pin
	pinMode(waterLevelPin, INPUT_PULLUP);
	
	// config pump pin and set pump initial off
	pinMode(pumpPin, OUTPUT);
	digitalWrite(pumpPin, false);

	// connect to wifi network
	connectWiFi();

	// initialize RTC and set firt measurement alarm
	initRTC();
}

// the loop function runs over and over again until power down or reset
void loop() {
	// nothing to do here. everything is controlled by RTC alarm events.
}





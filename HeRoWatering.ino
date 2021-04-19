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

// ArduinoBlue Library
//#include <ArduinoBlue.h>

RTCZero rtc;
WiFiClient client;
WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "de.pool.ntp.org", 0, 0);
EMailSender emailSender(MAIL, MAILPASS);

// the setup function runs once when you press reset or power the board
void setup() {
	// Debugging
	Serial.begin(115200);

	connectWiFi();
	initRTC();

	logToDatabase(123, "Test123", 456, "Test456", false, true);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	//rtc.standbyMode();    // sleep until next alarm match
}





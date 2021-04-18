/*
 Name:		HeRoWatering.ino
 Created:	14.04.2021 19:25:55
 Author:	Leonhard Hesse
*/

#include <Credentials.h>

// Sleep Modes and Time for Arduino Nano 33 IoT
#include <RTCZero.h>
#include <time.h>

// Libraries for Wifi
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

// Sunrise and Sunset Library
#include <Dusk2Dawn.h>

// SQL MariaDB Library
#include <MySQL_Generic_WiFiNINA.h>

// ArduinoBlue Library
#include <ArduinoBlue.h>

// global variable for rtc
RTCZero rtc;

extern const int offsetMin;

// the setup function runs once when you press reset or power the board
void setup() {
	// get extern variables
	extern const float longitude;
	extern const float latitude;
	
	// connect to WiFi network
	int status = WL_IDLE_STATUS;
	extern const char* SSID;
	extern const char* PASS;

	while (status != WL_CONNECTED) {
		status = WiFi.begin(SSID, PASS);
		delay(2000);
	}

	WiFi.lowPowerMode();	// enable WiFi low power mode

	// RTC init
	rtc.begin();
	rtc.setEpoch(WiFi.getTime());	// set rtc from internet time
	setMeasureAlarm();
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	//rtc.standbyMode();    // sleep until next alarm match
}

void ISR_RTC_Measurement() {
	// ToDo: 
	// - read plant sensors
	// - read waterlevel sensor
	// - logging sensor values und decision
	// - watering is necessary and possible
	//		yes:	- start watering
	//				- set RTC for stopping watering
	// 	    necessary but impossible:
	//				- send Email notification - ERROR
	//				- call setMeasureAlarm
	//		no:		- call setMeasureAlarm
}

void ISR_RTC_StopWatering() {
	// ToDo:
	// - stop watering
	// - call setMeasureAlarm
}

void setMeasureAlarm() {
	// get todays sunrise and sunset
	Dusk2Dawn regensburg(longitude, latitude, -2);
	int sunriseMins = regensburg.sunrise(rtc.getYear(), rtc.getMonth(), rtc.getDay(), true);
	int sunsetMins = regensburg.sunset(rtc.getYear(), rtc.getMonth(), rtc.getDay(), true);
	int timeMins = getMinutesSinceMidnight();

	// DEBUG:
	char strSunrise[6];
	Dusk2Dawn::min2str(strSunrise, sunriseMins);
	char strSunset[6];
	Dusk2Dawn::min2str(strSunset, sunsetMins);

	// check what event is next
	if (timeMins >= sunsetMins || timeMins < sunriseMins) {
		// sunrise is next - set alarm to sunrise time + offset
		rtc.setAlarmTime(sunriseMins / 60, sunriseMins % 60 + offsetMin, 0);
	}
	else 	{
		// sunset is next - set alarm to sunset time - offset
		rtc.setAlarmTime(sunsetMins / 60, sunsetMins % 60 - offsetMin, 0);
	}
	
	rtc.enableAlarm(rtc.MATCH_HHMMSS);
	rtc.attachInterrupt(ISR_RTC_Measurement);

	//rtc.standbyMode();
}

int getMinutesSinceMidnight() {
	int minutes;
	minutes = rtc.getHours() * 60;
	minutes += rtc.getMinutes();
	return minutes;
}

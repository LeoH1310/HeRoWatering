// 
// 
// 

//#define TESTING

#include "RtcAlarmManager.h"
#include "Logging.h"
#include <CuteBuzzerSounds.h>

SunSet sun;
alarmFlag alrFlag;

void initRTC() {
	rtc.begin();
	timeClient.begin();
	updateRTC();
	alrFlag = initial;
	sun.setPosition(latitude, longitude, timezone);
	setNextMeasureAlarm();
}

void getNextAlarmFlag() {
	long sunriseSecs;
	long sunsetSecs;
	long timeSecs;

	switch (alrFlag) 	{		
	case initial:
		// get todays sunrise and sunset
		sun.setCurrentDate(rtc.getYear(), rtc.getMonth(), rtc.getDay());
		sunriseSecs = sun.calcSunrise() * 60;
		sunsetSecs = sun.calcSunset() * 60;
		timeSecs = getSecondsSinceMidnight();
		// if sunrise is next - set alrFlag to sunrise
		if (timeSecs >= sunsetSecs - (offsetMin * 60) || timeSecs < sunriseSecs + (offsetMin * 60)) alrFlag = sunrise;
		// else sunset is next - set alrFlag to sunset
		else alrFlag = sunset;
		break;

	case sunrise:
		alrFlag = sunset;
		break;

	case sunset:
		alrFlag = update;
		break;

	case update:
		alrFlag = sunrise;
		break;
	}
}

void setNextMeasureAlarm() {
	getNextAlarmFlag();
	// get todays sunrise and sunset
	sun.setCurrentDate(rtc.getYear(), rtc.getMonth(), rtc.getDay());
	long sunriseSecs = sun.calcSunrise() * 60;
	long sunsetSecs = sun.calcSunset() * 60;
	long timeSecs = getSecondsSinceMidnight();

	char time[8];
	sprintf(time, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
	myPrint("Now is: ");
	myPrintln(time);
	char date[10];
	sprintf(date, "%02d.%02d.%02d", rtc.getDay(), rtc.getMonth(), rtc.getYear());
	myPrint("Today is: ");
	myPrintln(date);
	char sunriseStr[5];
	sprintf(sunriseStr, "%02d:%02d", (int)(sunriseSecs / 3600), (int)(sunriseSecs / 60) - ((int)(sunriseSecs / 3600) * 60));
	myPrint("Todays Sunrise: ");
	myPrintln(sunriseStr);
	char sunsetStr[5];
	sprintf(sunsetStr, "%02d:%02d", (int)(sunsetSecs / 3600), (int)(sunsetSecs / 60) - ((int)(sunsetSecs / 3600) * 60));
	myPrint("Todays Sunset: ");
	myPrintln(sunsetStr);

	// check what event is next
	int alarmMins = 0;
	switch (alrFlag) {
	case sunrise:
		alarmMins = (sunriseSecs / 60) + offsetMin;
		rtc.setAlarmTime(alarmMins / 60, alarmMins % 60, 0);
		rtc.enableAlarm(rtc.MATCH_HHMMSS);
		rtc.attachInterrupt(ISR_RTC_Measurement);
		break;

	case sunset:
		alarmMins = (sunsetSecs / 60) - offsetMin;
		rtc.setAlarmTime(alarmMins / 60, alarmMins % 60, 0);
		rtc.enableAlarm(rtc.MATCH_HHMMSS);
		rtc.attachInterrupt(ISR_RTC_Measurement);
		break;

	case update:
		rtc.setAlarmTime(2, 0, 0);
		rtc.enableAlarm(rtc.MATCH_HHMMSS);
		rtc.attachInterrupt(ISR_UPDATE_WEATHER);
		break;	
	}


	#ifdef TESTING
		rtc.setAlarmEpoch(rtc.getEpoch() + 60*2);	// RTC Alarm every 2 minutes
	#endif	// TESTING

	char alarmTime[5];
	sprintf(alarmTime, "%02d:%02d:%02d", rtc.getAlarmHours(), rtc.getAlarmMinutes(), rtc.getAlarmSeconds());
	myPrint("Next alarm: ");
	myPrintln(alarmTime);
	myPrintln("**********EndTime**********");
}

long getSecondsSinceMidnight() {
	long seconds;
	seconds = rtc.getHours() * 60 * 60;
	seconds += rtc.getMinutes() * 60;
	seconds += rtc.getSeconds();
	return seconds;
}

void updateRTC() {
	// check if WiFi connection is established, if not reconnect
	checkWifiConnection();

	if (timeClient.update()) {
		myPrintln("updating rtc...");
		rtc.setEpoch(timeClient.getEpochTime());	// set rtc system time from internet time
		char time[8];
		sprintf(time, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
		myPrint("Time after update: ");
		myPrintln(time);
	}
}

void ISR_RTC_Measurement() {
	if (!flag_runMeasurement) flag_runMeasurement = true;
}

void ISR_RTC_StopWatering() {
	if (!flag_stopWatering) flag_stopWatering = true;
}

void ISR_UPDATE_WEATHER() {
	if (!flag_updateWeather) flag_updateWeather = true;
}

void runMeasurement(MoistureSensor* sensorErdbeeren, MoistureSensor* sensorTomaten) {
	myPrintln("measurement started..");

	// read waterlevel sensor
	bool waterLevel = checkWaterLevel();
	// read moisture sensors
	Moisture moistureErdbeeren = sensorErdbeeren->getValue();
	Moisture moistureTomaten = sensorTomaten->getValue();

	int waterAmount = checkForWatering(moistureErdbeeren, moistureTomaten);

	myPrint("WaterLevel: ");
	myPrintln(waterLevel);
	myPrint("WaterAmount: ");
	myPrintln(waterAmount);

	myPrint("Sens1ErdbeereVal: ");
	myPrint(moistureErdbeeren.rawValue);
	myPrint(" #");
	myPrintln(moistureErdbeeren.getName());

	myPrint("Sens2TomateVal: ");
	myPrint(moistureTomaten.rawValue);
	myPrint(" #");
	myPrintln(moistureTomaten.getName());
		
	myPrintln("**********EndData**********");

	if (waterLevel) {	// water level OK
		if (waterAmount > 0) {	// watering necessary
			// set rtc alarm to stop watering
			long stopTime = rtc.getEpoch() + (wateringTimeMin * 60 * waterAmount);
			rtc.setAlarmEpoch(stopTime);
			rtc.enableAlarm(rtc.MATCH_HHMMSS);
			rtc.attachInterrupt(ISR_RTC_StopWatering);

			myPrintln("watering started..");
			// play start sound
			cute.play(S_CONNECTION);
			// start watering
			digitalWrite(pumpPin, true);
		}
		else {	// no watering necessary
			myPrintln("no watering needed :)");
			// play happy sound
			cute.play(S_HAPPY);

			// set rtc alarm for next measuring
			setNextMeasureAlarm();
		}
	}
	else {	// water level low
		myPrintln("water level low!");
		// play sad sound
		cute.play(S_SAD);
		// send email notification
		bool sendStatus = sendEmail("AquaHeRo: Wasserstand kritisch!", "Der Wasserstand ist zu niedrig, um AquaHeRo zu starten.", "hesseleo1310@gmail.com");
		// set rtc alarm for next measuring
		setNextMeasureAlarm();
	}

	// log measurement and decissions to database
	logToDatabase(moistureErdbeeren.rawValue, moistureErdbeeren.getName(), moistureTomaten.rawValue, moistureTomaten.getName(), waterAmount, waterLevel);
}

void stopWatering() {
	// stop watering
	digitalWrite(pumpPin, false);

	myPrintln("watering stopped.");

	// play finishing sound
	cute.play(S_MODE3);

	// set rtc alarm for next measuring
	setNextMeasureAlarm();
}

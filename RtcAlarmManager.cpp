// 
// 
// 

#include "RtcAlarmManager.h"

MoistureSensor* sensorErdbeeren = new MoistureSensor(sensor1Pin, sensor1Air, sensor1Water);
MoistureSensor* sensorTomaten = new MoistureSensor(sensor2Pin, sensor2Air, sensor2Water);

void initRTC() {
	rtc.begin();
	timeClient.begin();
	updateRTC();

	char time[8];
	sprintf(time, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
	Serial.print("Now is: ");
	Serial.println(time);

	setNextMeasureAlarm();
}

void setNextMeasureAlarm() {
	// get todays sunrise and sunset
	SunSet sun;
	sun.setPosition(latitude, longitude, timezone);
	sun.setCurrentDate(rtc.getYear(), rtc.getMonth(), rtc.getDay());
	int sunriseMins = sun.calcSunrise();
	int sunsetMins = sun.calcSunset();
	int timeMins = getMinutesSinceMidnight();

	char date[10];
	sprintf(date, "%02d.%02d.%02d", rtc.getDay(), rtc.getMonth(), rtc.getYear());
	Serial.print("Today is: ");
	Serial.println(date);
	char sunriseStr[5];
	sprintf(sunriseStr, "%02d:%02d", sunriseMins / 60, sunriseMins % 60);
	Serial.print("Todays Sunrise: ");
	Serial.println(sunriseStr);
	char sunsetStr[5];
	sprintf(sunsetStr, "%02d:%02d", sunsetMins / 60, sunsetMins % 60);
	Serial.print("Todays Sunset: ");
	Serial.println(sunsetStr);


	// check what event is next
	if (timeMins >= sunsetMins || timeMins < sunriseMins) {
		// sunrise is next - set alarm to sunrise time + offset
		rtc.setAlarmTime(sunriseMins / 60, sunriseMins % 60 + offsetMin, 0);
		Serial.println("Sunrise is next.");
	}
	else {
		// sunset is next - set alarm to sunset time - offset
		rtc.setAlarmTime(sunsetMins / 60, sunsetMins % 60 - offsetMin, 0);
		Serial.println("Sunset is next.");
	}

	// update rtc time from web to keep in sync
	updateRTC();

	rtc.enableAlarm(rtc.MATCH_HHMMSS);
	rtc.attachInterrupt(ISR_RTC_Measurement);
}

int getMinutesSinceMidnight() {
	int minutes;
	minutes = rtc.getHours() * 60;
	minutes += rtc.getMinutes();
	return minutes;
}

void updateRTC() {
	timeClient.update();
	unsigned long localTime = timeClient.getEpochTime() + (timezone * 3600);
	rtc.setEpoch(localTime);	// set rtc system time from internet time
}

void ISR_RTC_Measurement() {
	// read waterlevel sensor
	bool waterLevel = checkWaterLevel();
	// read moisture sensors
	Moisture moistureErdbeeren = sensorErdbeeren->getValue();
	Moisture moistureTomaten = sensorTomaten->getValue();

	bool action = checkForWatering(moistureErdbeeren, moistureTomaten);
	if (waterLevel) {	// water level OK
		if (action) {	// watering necessary
			// set rtc alarm to stop watering
			long stopTime = rtc.getEpoch() + (wateringTimeMin * 60);
			rtc.setAlarmEpoch(stopTime);
			rtc.enableAlarm(rtc.MATCH_HHMMSS);
			rtc.attachInterrupt(ISR_RTC_StopWatering);
			// start watering
			digitalWrite(pumpPin, true);
		}
		else {	// no watering necessary
			// set rtc alarm for next measuring
			setNextMeasureAlarm();
		}
	}
	else {	// water level low
		// send email notification
		bool sendStatus = sendEmail("AquaHeRo: Wasserstand kritisch!", "Der Wasserstand ist zu niedrig, um AquaHeRo zu starten.", "hesseleo1310@gmail.com");
		// set rtc alarm for next measuring
		setNextMeasureAlarm();
	}

	// log measurement and decissions to database
	logToDatabase(moistureErdbeeren.rawValue, moistureErdbeeren.getName(), moistureTomaten.rawValue, moistureTomaten.getName(), action, waterLevel);

	// sleep arduino till next rtc event
	rtc.standbyMode();
}

void ISR_RTC_StopWatering() {
	// stop watering
	digitalWrite(pumpPin, false);

	// set rtc alarm for next measuring
	setNextMeasureAlarm();

	// sleep arduino till next rtc event
	rtc.standbyMode();
}

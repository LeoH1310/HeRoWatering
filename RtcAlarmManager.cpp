// 
// 
// 

//#define TESTING

#include "RtcAlarmManager.h"
#include <CuteBuzzerSounds.h>

MoistureSensor* sensorErdbeeren = new MoistureSensor(sensor1Pin, sensor1Air, sensor1Water);
MoistureSensor* sensorTomaten = new MoistureSensor(sensor2Pin, sensor2Air, sensor2Water);

void initRTC() {
	rtc.begin();
	timeClient.begin();
	updateRTC();
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

	char time[8];
	sprintf(time, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
	Serial.print("Now is: ");
	Serial.println(time);
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
	int alarmMins = 0;
	if (timeMins >= sunsetMins - offsetMin || timeMins < sunriseMins + offsetMin) {
		// sunrise is next - set alarm to sunrise time + offset
		alarmMins = sunriseMins + offsetMin;
		rtc.setAlarmTime(alarmMins / 60, alarmMins % 60, 0);
	}
	else {
		// sunset is next - set alarm to sunset time - offset
		alarmMins = sunsetMins - offsetMin;
		rtc.setAlarmTime(alarmMins / 60, alarmMins % 60, 0);
	}

	#ifdef TESTING
		rtc.setAlarmEpoch(rtc.getEpoch() + 60*2);	// RTC Alarm evry 2 minutes
	#endif	// TESTING

	char alarmTime[5];
	sprintf(alarmTime, "%02d:%02d:%02d", rtc.getAlarmHours(), rtc.getAlarmMinutes(), rtc.getAlarmSeconds());
	Serial.print("Next alarm: ");
	Serial.println(alarmTime);

	Serial.println("**********EndTime**********");

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
	// check if WiFi connection is established, if not reconnect
	checkWifiConnection();

	Serial.println("updating rtc...");
	bool status = timeClient.update();
	delay(200);

	int i = 1;
	while (!status && i < 5) {
		Serial.print("updating rtc failed #");
		Serial.print(i);
		Serial.println(". retrying...");
		status = timeClient.forceUpdate();
		delay(200);
		i++;
	}

	if (i < 5) {
		Serial.println("updating rtc successful :)");
		rtc.setEpoch(timeClient.getEpochTime() + (timezone * 3600));	// set rtc system time from internet time
		char time[8];
		sprintf(time, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
		Serial.print("Time after update: ");
		Serial.println(time);
	}
	// reboot if rtc update failed 5 times
	else reboot();
}

void ISR_RTC_Measurement() {
	// read waterlevel sensor
	bool waterLevel = checkWaterLevel();
	// read moisture sensors
	Moisture moistureErdbeeren = sensorErdbeeren->getValue();
	Moisture moistureTomaten = sensorTomaten->getValue();

	int waterAmount = checkForWatering(moistureErdbeeren, moistureTomaten);

	Serial.print("WaterLevel: ");
	Serial.println(waterLevel);
	Serial.print("WaterAmount: ");
	Serial.println(waterAmount);

	Serial.print("Sens1ErdbeereVal: ");
	Serial.print(moistureErdbeeren.rawValue);
	Serial.print(" #");
	Serial.println(moistureErdbeeren.getName());

	Serial.print("Sens2TomateVal: ");
	Serial.print(moistureTomaten.rawValue);
	Serial.print(" #");
	Serial.println(moistureTomaten.getName());
		
	Serial.println("**********EndData**********");

	if (waterLevel) {	// water level OK
		if (waterAmount > 0) {	// watering necessary
			// set rtc alarm to stop watering
			long stopTime = rtc.getEpoch() + (wateringTimeMin * 60 * waterAmount);
			rtc.setAlarmEpoch(stopTime);
			rtc.enableAlarm(rtc.MATCH_HHMMSS);
			rtc.attachInterrupt(ISR_RTC_StopWatering);

			Serial.println("watering started..");
			// play start sound
			cute.play(S_CONNECTION);
			// start watering
			digitalWrite(pumpPin, true);
		}
		else {	// no watering necessary

			Serial.println("no watering needed :)");
			// play happy sound
			cute.play(S_HAPPY);

			// set rtc alarm for next measuring
			setNextMeasureAlarm();
		}
	}
	else {	// water level low

		Serial.println("water level low!");
		// play sad sound
		cute.play(S_SAD);
		// send email notification
		bool sendStatus = sendEmail("AquaHeRo: Wasserstand kritisch!", "Der Wasserstand ist zu niedrig, um AquaHeRo zu starten.", "hesseleo1310@gmail.com");
		// set rtc alarm for next measuring
		setNextMeasureAlarm();
	}

	// log measurement and decissions to database
	logToDatabase(moistureErdbeeren.rawValue, moistureErdbeeren.getName(), moistureTomaten.rawValue, moistureTomaten.getName(), waterAmount, waterLevel);

	// sleep arduino till next rtc event
	//rtc.standbyMode();
}

void ISR_RTC_StopWatering() {
	// stop watering
	digitalWrite(pumpPin, false);

	Serial.println("watering stopped.");

	// play finishing sound
	cute.play(S_MODE3);

	// set rtc alarm for next measuring
	setNextMeasureAlarm();

	// sleep arduino till next rtc event
	//rtc.standbyMode();
}

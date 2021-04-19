// 
// 
// 

#include "RtcAlarmManager.h"

void initRTC() {
	rtc.begin();
	timeClient.begin();
	updateRTC();

	char time[8];
	sprintf(time, "%d:%d:%d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
	Serial.print("Now is: ");
	Serial.println(time);

	setMeasureAlarm();
}

void setMeasureAlarm() {
	// get todays sunrise and sunset
	SunSet sun;
	sun.setPosition(latitude, longitude, timezone);
	sun.setCurrentDate(rtc.getYear(), rtc.getMonth(), rtc.getDay());
	int sunriseMins = sun.calcSunrise();
	int sunsetMins = sun.calcSunset();
	int timeMins = getMinutesSinceMidnight();

	char date[10];
	sprintf(date, "%d.%d.%d", rtc.getDay(), rtc.getMonth(), rtc.getYear());
	Serial.print("Today is: ");
	Serial.println(date);
	char sunriseStr[5];
	sprintf(sunriseStr, "%d:%d", sunriseMins / 60, sunriseMins % 60);
	Serial.print("Todays Sunrise: ");
	Serial.println(sunriseStr);
	char sunsetStr[5];
	sprintf(sunsetStr, "%d:%d", sunsetMins / 60, sunsetMins % 60);
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

void updateRTC() {
	timeClient.update();
	unsigned long localTime = timeClient.getEpochTime() + (timezone * 3600);
	rtc.setEpoch(localTime);	// set rtc system time from internet time
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

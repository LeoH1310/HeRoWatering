// RtcAlarmManager.h

#pragma once

#include "arduino.h"

#include <NTPClient.h>
#include <RTCZero.h>
#include <time.h>
#include <sunset.h>

#include "SensorManager.h"
#include "DatabaseManager.h"
#include "EmailMangager.h"
#include "WiFiManager.h"

extern RTCZero rtc;
extern NTPClient timeClient;

extern const double longitude;
extern const double latitude;
extern const float timezone;
extern const int offsetMin;
extern const int wateringTimeMin;
extern const int pumpPin;

extern volatile boolean flag_runMeasurement;
extern volatile boolean flag_stopWatering;
extern volatile boolean flag_updateWeather;

void ISR_RTC_Measurement();
void ISR_RTC_StopWatering();

void initRTC();
void updateRTC();

void runMeasurement(MoistureSensor* sensorErdbeeren, MoistureSensor* sensorTomaten);
void stopWatering();

void setNextMeasureAlarm();
long getSecondsSinceMidnight();




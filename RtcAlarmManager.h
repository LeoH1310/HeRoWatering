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

enum alarmFlag { initial, sunrise, sunset, update };

extern RTCZero rtc;
extern NTPClient timeClient;

extern volatile boolean flag_runMeasurement;
extern volatile boolean flag_stopWatering;
extern volatile boolean flag_updateWeather;

void ISR_RTC_Measurement();
void ISR_RTC_StopWatering();
void ISR_UPDATE_WEATHER();

void initRTC();
void updateRTC();

void runMeasurement(MoistureSensor* sensorErdbeeren, MoistureSensor* sensorTomaten);
void stopWatering();

void setNextMeasureAlarm();
long getSecondsSinceMidnight();




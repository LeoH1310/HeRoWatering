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

void initRTC();
void setNextMeasureAlarm();
int getMinutesSinceMidnight();
void updateRTC();
void ISR_RTC_Measurement();
void ISR_RTC_StopWatering();


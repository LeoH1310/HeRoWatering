// RtcAlarmManager.h

#pragma once

#include "arduino.h"

#include <NTPClient.h>
#include <RTCZero.h>
#include <time.h>
#include <sunset.h>

extern RTCZero rtc;
extern NTPClient timeClient;

extern const double longitude;
extern const double latitude;
extern const float timezone;
extern const int offsetMin;

void initRTC();
void setMeasureAlarm();
int getMinutesSinceMidnight();
void updateRTC();
void ISR_RTC_Measurement();
void ISR_RTC_StopWatering();


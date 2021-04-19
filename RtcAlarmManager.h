// RtcAlarmManager.h

#pragma once

#include "arduino.h"

// Wifi for timesync
#include "DriverWiFi.h"
#include <NTPClient.h>

// Sleep Modes and Time for Arduino Nano 33 IoT
#include <RTCZero.h>
#include <time.h>

// sunrise sunset time
#include <sunset.h>

extern RTCZero rtc;
extern WiFiUDP ntpUdp;
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


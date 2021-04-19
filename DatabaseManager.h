// DatabaseManager.h

#pragma once

#include "arduino.h"

#include "RtcAlarmManager.h"
#include "WiFiManager.h"

#include <MySQL_Packet.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Cursor.h>
#include <MySQL_Connection.h>

extern RTCZero rtc;
extern WiFiClient client;

extern const IPAddress SQL_IP;
extern const uint16_t SQL_PORT;
extern const char* SQL_DB;
extern const char* SQL_TABLE;
extern char* SQL_USER;
extern char* SQL_PASS;

void logToDatabase(int raw1, char* txt1, int raw2, char* txt2, bool act, bool water);




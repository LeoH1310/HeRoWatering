// Logging.h

#pragma once

#include "arduino.h"
#include "WiFiManager.h"

extern WiFiServer telnetServer;

void myPrint(String log);
void myPrint(const char* log);
void myPrint(int log);
void myPrint(double log);
void myPrint(float log);

void myPrintln(String log);
void myPrintln(const char* log);
void myPrintln(int log);
void myPrintln(double log);
void myPrintln(float log);



// 
// 
// 
//#define TELNET

#include "Logging.h"

void myPrint(String log) {
	Serial.print(log);
#ifdef TELNET
	telnetServer.print(log);
#endif
}

void myPrint(const char* log) {
	Serial.print(log);
	#ifdef TELNET
	telnetServer.print(log);
	#endif
}

void myPrint(int log) {
	Serial.print(log);
	#ifdef TELNET
	telnetServer.print(log);
	#endif
}

void myPrint(double log) {
	Serial.print(log);
	#ifdef TELNET
	telnetServer.print(log);
	#endif
}

void myPrint(float log) {
	Serial.print(log);
	#ifdef TELNET
	telnetServer.print(log);
	#endif
}

void myPrintln(String log) {
	Serial.println(log);
#ifdef TELNET
	telnetServer.println(log);
#endif
}

void myPrintln(const char* log) {
	Serial.println(log);
	#ifdef TELNET
	telnetServer.println(log);
	#endif
}

void myPrintln(int log) {
	Serial.println(log);
	#ifdef TELNET
	telnetServer.println(log);
	#endif
}

void myPrintln(double log) {
	Serial.println(log);
	#ifdef TELNET
	telnetServer.println(log);
	#endif
}

void myPrintln(float log) {
	Serial.println(log);
	#ifdef TELNET
	telnetServer.println(log);
	#endif
}

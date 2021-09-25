/*
 Name:		HeRoWatering.ino
 Created:	14.04.2021 19:25:55
 Author:	Leonhard Hesse
*/

//#define DEBUGDELAY
//#define TELNET

#include "Credentials.h"
#include "WiFiManager.h"
#include "RtcAlarmManager.h"
#include "EmailMangager.h"
#include "SensorManager.h"
#include "DatabaseManager.h"
#include "MoistureSensor.h"
#include "WeatherAPI.h"
#include "Logging.h"

#include <Sounds.h>
#include <CuteBuzzerSounds.h>
#include <Sodaq_wdt.h>

extern const int waterLevelPin;
extern const int BUZZER_PIN;
extern const unsigned long updateInterval;

RTCZero rtc;
WiFiServer telnetServer(23);
WiFiClient telnetClient;
WiFiClient client;
WiFiSSLClient clientSecure;
WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "de.pool.ntp.org", timezone * 3600, updateInterval);

MoistureSensor* sensorErdbeeren = new MoistureSensor(sensor1Pin, sensor1Air, sensor1Water);
MoistureSensor* sensorTomaten = new MoistureSensor(sensor2Pin, sensor2Air, sensor2Water);

Weather* tomorrow = new Weather();

extern volatile boolean flag_runMeasurement = false;
extern volatile boolean flag_stopWatering = false;
extern volatile boolean flag_updateWeather = false;

boolean alreadyConnected = false; // whether or not the telnetClient was connected previously

uint64_t startMillis = millis();

// the setup function runs once when you press reset or power the board
void setup() {

	// init for serial debugging
	delay(1000);
	Serial.begin(115200);

	#ifdef DEBUGDELAY	// Debug startup delay
		Serial.println();
		Serial.println("DEBUG: start up delay..");
		for (int i = 10; i > 0; i--) {
			Serial.println(i);
			delay(1000);
		}
	#endif // DEBUG

	Serial.println();
	Serial.println("**********start up***********");

	// cofig waterlevel pin
	pinMode(waterLevelPin, INPUT_PULLUP);
	
	// config pump pin and set pump initial off
	pinMode(pumpPin, OUTPUT);
	digitalWrite(pumpPin, false);

	// config buzzer pin
	cute.init(BUZZER_PIN);

	// connect to wifi network
	connectWiFi();
	WiFi.lowPowerMode();	// enable WiFi low power mode

	#ifdef TELNET
	telnetServer.begin();
	delay(1000);
	myPrintln("**********start up telnet ***********");
	#endif 

	// initialize RTC and set firt measurement alarm
	initRTC();
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (flag_runMeasurement) {
		rtc.disableAlarm();
		rtc.detachInterrupt();
		delay(1000);	// wait 1 second
		flag_runMeasurement = false;
		runMeasurement(sensorErdbeeren, sensorTomaten);
	}
	if (flag_stopWatering) {
		rtc.disableAlarm();
		rtc.detachInterrupt();
		delay(1000);	// wait 1 second
		flag_stopWatering = false;
		stopWatering();
	}
	if (flag_updateWeather) {
		rtc.disableAlarm();
		rtc.detachInterrupt();
		delay(1000);	// wait 1 second
		flag_updateWeather = false;
		updateRTC();
		delay(100);
		//tomorrow->update(clientSecure);
		setNextMeasureAlarm();
	}

	#ifdef TELNET
	// check for a new client:
	telnetClient = telnetServer.available();

	// when the client sends the first byte, say hello:
	if (telnetClient) {
		if (!alreadyConnected) {
			// clear out the input buffer:
			telnetClient.flush();
			telnetClient.println("Welcome to AquHeRo DEBUGGING!");
			telnetClient.println("e for exit");
			telnetClient.println("r for reset");
			alreadyConnected = true;
		}

		if (telnetClient.available() > 0) {
			// read the bytes incoming from the client:
			char input = telnetClient.read();

			if (input == 'r') {
				telnetClient.println();
				telnetClient.println("reboot...");
				telnetClient.println();
				telnetClient.println("bye bye!");
				telnetClient.stop(); // cleans up memory
				telnetClient = telnetServer.available(); // evaluates to false if no connection
				alreadyConnected = false;
				reboot();
			}
			if (input == 'e') {
				telnetClient.println();
				telnetClient.println("bye bye!");
				telnetClient.stop(); // cleans up memory
				telnetClient = telnetServer.available(); // evaluates to false if no connection
				alreadyConnected = false;
			}
		}
	}
	#endif 
}

void reboot() {
	myPrintln("reset triggert ..");
	cute.play(S_CUDDLY);
	sodaq_wdt_enable(WDT_PERIOD_1DIV2);
	delay(1000);
	sodaq_wdt_reset();
}

/*
 S_CONNECTION   S_DISCONNECTION S_BUTTON_PUSHED
 S_MODE1        S_MODE2         S_MODE3
 S_SURPRISE     S_OHOOH         S_OHOOH2
 S_CUDDLY       S_SLEEPING      S_HAPPY
 S_SUPER_HAPPY  S_HAPPY_SHORT   S_SAD
 S_CONFUSED     S_FART1         S_FART2
 S_FART3        S_JUMP 20
 */





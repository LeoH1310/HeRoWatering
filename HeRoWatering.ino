/*
 Name:		HeRoWatering.ino
 Created:	14.04.2021 19:25:55
 Author:	Leonhard Hesse
*/

//#define DEBUGDELAY

#include "WeatherAPI.h"
#include <Sodaq_wdt.h>
#include "Credentials.h"
#include "WiFiManager.h"
#include "RtcAlarmManager.h"
#include "EmailMangager.h"
#include "SensorManager.h"
#include "DatabaseManager.h"
#include "MoistureSensor.h"

#include <Sounds.h>
#include <CuteBuzzerSounds.h>

extern const int waterLevelPin;
extern const int BUZZER_PIN;

RTCZero rtc;
WiFiClient client;
WiFiSSLClient clientSecure;
WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "de.pool.ntp.org", timezone * 3600, 86400000);
EMailSender emailSender(MAIL, MAILPASS);

MoistureSensor* sensorErdbeeren = new MoistureSensor(sensor1Pin, sensor1Air, sensor1Water);
MoistureSensor* sensorTomaten = new MoistureSensor(sensor2Pin, sensor2Air, sensor2Water);

Weather* tomorrow = new Weather();

extern volatile boolean flag_runMeasurement = false;
extern volatile boolean flag_stopWatering = false;
extern volatile boolean flag_updateWeather = false;

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

	// initialize RTC and set firt measurement alarm
	initRTC();

	// Testing
	//flag_updateWeather = true;
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (flag_runMeasurement) {
		flag_runMeasurement = false;
		runMeasurement(sensorErdbeeren, sensorTomaten);
	}
	if (flag_stopWatering) {
		flag_stopWatering = false;
		stopWatering();
	}
	if (flag_updateWeather) {
		flag_updateWeather = false;
		tomorrow->update(clientSecure);
	}
}

void reboot() {
	Serial.println("reset triggert ..");
	cute.play(S_CUDDLY);
	sodaq_wdt_enable(WDT_PERIOD_1DIV2);
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





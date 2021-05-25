/*
 Name:		HeRoWatering.ino
 Created:	14.04.2021 19:25:55
 Author:	Leonhard Hesse
*/

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
WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "de.pool.ntp.org", 0, 0);
EMailSender emailSender(MAIL, MAILPASS);

// the setup function runs once when you press reset or power the board
void setup() {

	// init for serial debugging
	Serial.begin(115200);
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
}

// the loop function runs over and over again until power down or reset
void loop() {
	// nothing to do here. everything is controlled by RTC alarm events.
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





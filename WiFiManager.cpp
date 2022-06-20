// 
// 
// 

#include "WiFiManager.h"
#include "Logging.h"
#include <CuteBuzzerSounds.h>

void connectWiFi() {
	// connect to WiFi network
	int status = WL_IDLE_STATUS;
	WiFi.setHostname(HOST);

	Serial.println("connecting WiFi...");
	status = WiFi.begin(SSID, PASS);
	delay(2000);

	int i = 1;
	while (status != WL_CONNECTED && i < 5) {
		Serial.print("WiFi connection failed #");
		Serial.print(i);
		Serial.println(". reconnecting...");
		cute.play(S_DISCONNECTION);
		status = WiFi.begin(SSID, PASS);
		delay(2000);
		i++;
	}

	if (i < 5) {
		Serial.println("WiFi connected :)");
		cute.play(S_MODE3);
	}
	// reboot if wifi connection failed 5 times
	else reboot();
}

void checkWifiConnection() {
	if (WiFi.status() != WL_CONNECTED) connectWiFi();
}
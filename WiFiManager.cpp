// 
// 
// 

#include "WiFiManager.h"

void connectWiFi() {
	// connect to WiFi network
	int status = WL_IDLE_STATUS;
	WiFi.setHostname(HOST);

	while (status != WL_CONNECTED) {
		status = WiFi.begin(SSID, PASS);
		Serial.println("WiFi connection failed. Reconnecting...");
		delay(2000);
	}

	// DEBUG
	Serial.println("WiFi connected :)");

	//WiFi.lowPowerMode();	// enable WiFi low power mode
}



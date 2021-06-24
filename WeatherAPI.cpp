// 
// 
// 

#include "WeatherAPI.h"

void Weather::update(WiFiSSLClient client) {
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial: 
    if (client.connect(server, 443)) {
        // Make a HTTP request: 
        client.print("GET /data/2.5/onecall?");
        client.print(location);
        client.print("&units=metric");
        client.print("&exclude=minutely,hourly,alerts");
        client.println("&appid=" + apiKey);
        client.print("&cnt=3");
        client.println("Host: api.openweathermap.org");
        Serial.println("connected to api.openweathermap.org");
        client.println("Connection: close");
        client.println();
    }
    else {
        Serial.println("unable to connect to api.openweathermap.org");
    }
    delay(1000);
    String line = "";
    Serial.println(client.available());

    //Wait for the answer, max 2 sec.
    uint64_t startMillis = millis();
    while (client.available() == 0) {
        if (millis() - startMillis > 2000) {
            Serial.println("Client timeout");
            client.stop();
            return;
        }
    }

    while(client.available()) {
        line = client.readStringUntil('\r');
        client.stop();
        Serial.println(line); 
        Serial.println("parsing weather values...");
        //create a json buffer where to store the json data 
        ARDUINOJSON_NAMESPACE::StaticJsonDocument<8192> jsonDoc;
        auto error = ARDUINOJSON_NAMESPACE::deserializeJson(jsonDoc, line);
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        }
        Serial.println("parsing weather values DONE");
        // set weather data
        this->date = jsonDoc["daily"][1]["dt"];
        this->sunrise = jsonDoc["daily"][1]["sunrise"];
        this->sunset = jsonDoc["daily"][1]["sunset"];
        this->temp = jsonDoc["daily"][1]["temp"]["day"];
        this->rainProbability = jsonDoc["daily"][0]["pop"];
        if (this->rainProbability > 0.0) this->rainAmount = jsonDoc["daily"][0]["rain"];
        else this->rainAmount = 0.0;

        // print weather data
        Serial.print("Weather Date: ");
        Serial.print(ctime(&this->date));
        Serial.print("Sunrise: ");
        Serial.print(ctime(&this->sunrise));
        Serial.print("Sunset: ");
        Serial.print(ctime(&this->sunset));
        Serial.print("Temperature: ");
        Serial.print(this->temp);
        Serial.println(" C");
        Serial.print("Rain Prop.: ");
        Serial.print((int) (this->rainProbability * 100));
        Serial.println(" %");
        Serial.print("Rain Amount: ");
        Serial.print(this->rainAmount);
        Serial.println(" mm");
    }
}

ARDUINOJSON_NAMESPACE::StaticJsonDocument<5000>* getWeather(WiFiClient client) {
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial: 
    if (client.connect(server, 80)) {
        Serial.println("connected to api.openweathermap.org");
        // Make a HTTP request: 
        client.print("GET /data/2.5/onecall?");
        client.print(location);
        client.print("&units=metric");
        client.print("&exclude = minutely, hourly, alerts");
        client.println("&appid=" + apiKey);
        //client.print("&cnt=3");
        client.println("Host: api.openweathermap.org");
        client.println("Connection: close");
        client.println();
    }
    else {
        Serial.println("unable to connect to api.openweathermap.org");
    }
    delay(1000);
    String line = "";
    while (client.connected()) {
        line = client.readStringUntil('\n');
        //Serial.println(line); 
        Serial.println("parsing weather values");
        //create a json buffer where to store the json data 
        ARDUINOJSON_NAMESPACE::StaticJsonDocument<5000> doc;
        auto error = ARDUINOJSON_NAMESPACE::deserializeJson(doc, line);
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return nullptr;
        }
        return &doc;
    }
}
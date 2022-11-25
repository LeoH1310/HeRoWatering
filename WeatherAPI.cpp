// 
// 
// 

#include "WeatherAPI.h"
#include "Logging.h"

void Weather::update(WiFiSSLClient client) {
    Serial.println("\nStarting connection to weather API...");
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
        myPrintln("connected to api.openweathermap.org");
        client.println("Connection: close");
        client.println();
    }
    else {
        myPrintln("unable to connect to api.openweathermap.org");
    }
    delay(1000);
    String line = "";

    //Wait for the answer, max 2 sec.
    uint64_t startMillis = millis();
    while (client.available() == 0) {
        if (millis() - startMillis > 2000) {
            myPrintln("Client timeout");
            client.stop();
            return;
        }
    }

    while(client.available()) {
        line = client.readStringUntil('\r');
        client.stop();
        //Serial.println(line); 
        myPrintln("parsing weather values...");
        //create a json buffer where to store the json data 
        ARDUINOJSON_NAMESPACE::StaticJsonDocument<8192> jsonDoc;
        auto error = ARDUINOJSON_NAMESPACE::deserializeJson(jsonDoc, line);
        if (error) {
            myPrint("deserializeJson() failed with code ");
            myPrintln(error.c_str());
            return;
        }
        myPrintln("parsing weather values DONE");
        // set weather data
        this->date = jsonDoc["daily"][1]["dt"];
        this->sunrise = jsonDoc["daily"][1]["sunrise"];
        this->sunset = jsonDoc["daily"][1]["sunset"];
        this->temp = jsonDoc["daily"][1]["temp"]["day"];
        this->rainProbability = jsonDoc["daily"][0]["pop"];
        if (this->rainProbability > 0.0) this->rainAmount = jsonDoc["daily"][0]["rain"];
        else this->rainAmount = 0.0;

        // print weather data
        myPrint("Weather Date: ");
        myPrintln(ctime(&this->date));
        myPrint("Sunrise: ");
        myPrintln(ctime(&this->sunrise));
        myPrint("Sunset: ");
        myPrintln(ctime(&this->sunset));
        myPrint("Temperature: ");
        myPrint(this->temp);
        myPrintln(" C");
        myPrint("Rain Prop.: ");
        myPrint((int) (this->rainProbability * 100));
        myPrintln(" %");
        myPrint("Rain Amount: ");
        myPrint(this->rainAmount);
        myPrintln(" mm");
    }
}
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "AUT-Sensor";
const char* password = "AuTS3nS3";

//const String apiKEY = "e1b26c0d02aa985473e33f8bb695d756";
//const String apiURL = "http://api.openweathermap.org/data/2.5/forecast?id=2193734&APPID";
//const String apiAUTHENTICATED = String(apiURL + "=" + apiKEY);
//const String apiAUTHENTICATED = "http://api.apixu.com/v1/forecast.json?key=82efe492322b4aef83a223228171110&q=Paris&days=1";
const String apiAUTHENTICATED = "http://api.wunderground.com/api/03f7849bfa25a1b7/conditions/q/CA/San_Francisco.json";
 
void setup () {
 
  Serial.begin(2000000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected!");
    HTTPClient http;
    http.begin(apiAUTHENTICATED);
    int httpCode = http.GET();
    Serial.println(httpCode);
    if (httpCode > 0) {
      Serial.println("Getting payload...");
      String payload = http.getString();
      Serial.println(payload);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      if (!root.success()) {
        Serial.println("jsonBuffer.parseObject() failed.");
      }
      root.prettyPrintTo(Serial);
    }
    http.end();
    delay(60000);
  }
  delay(100);
}

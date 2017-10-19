#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "AUT-Sensor";
const char* password = "AuTS3nS3";
String apiAUTHENTICATED = "http://api.wunderground.com/api/03f7849bfa25a1b7/hourly/q/NZ/Auckland.json";
const boolean wifi = false;

const String items[] = {"Rain Jacket", "Sunglasses", "Warm Sweather"};
int number = 0;

void setup() {
  Serial.begin(115200);
  if (wifi) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting..");
    }
  }
}

void loop() {
  if (wifi) {
    wifiEnabled();
  } else {
    wifiDisabled();
  }
  delay(10000);
  Serial.begin(115200);
}

String forecastItem() {
  return "Sunglasses";
}

void wifiEnabled() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected! D1 online...");
    HTTPClient httpClient;
    httpClient.begin(apiAUTHENTICATED);
    int httpReturnValue = httpClient.GET();
    Serial.println(httpReturnValue);
    if (httpReturnValue > 0) {
      Serial.println("Getting payload...");
      String payload = httpClient.getString();
      Serial.println(payload);
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      if (!root.success()) {
        Serial.println("jsonBuffer.parseObject() failed.");
      }
      root.prettyPrintTo(Serial);
    } else {
      Serial.println("Something happened --> Could not find API.");
    }
    httpClient.end();
  }
}

void wifiDisabled() {
  
  char json[] = "{\"hourly_forecast\":[{\"temp\":{\"english\":\"61\",\"metric\":\"16\"},\"condition\":\"Partly Cloudy\"},{\"temp\":{\"english\":\"59\",\"metric\":\"15\"},\"condition\":\"Partly Cloudy\"}]}";
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  for (int i = 0; i < 2; i++) {
    root["hourly_forecast"][i]["temp"]["metric"].printTo(Serial);
    root["hourly_forecast"][i]["condition"].printTo(Serial);
  }
  Serial.print("\n");  
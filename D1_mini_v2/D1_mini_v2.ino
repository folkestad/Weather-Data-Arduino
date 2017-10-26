#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "AUT-Sensor";
const char* password = "AuTS3nS3";
String apiAUTHENTICATED = "http://api.wunderground.com/api/03f7849bfa25a1b7/forecast/q/NZ/Auckland.json";
const boolean wifi = true;

void setup() {

  // start serial connection
  Serial.begin(115200);

  // start wifi if we want to
  if (wifi) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      //Serial.println("Connecting..");
    }
  }
}

void loop() {
  
  // forecast from api values or local values
  if (wifi) {
    wifiEnabled();
  } else {
    wifiDisabled();
  }

  // only get data from api once a minute
  delay(60000);

  // reset serial connection to clear cache buffer
  Serial.begin(115200);
  
}

void forecastItem(int highTemp, int lowTemp, String condition) {
  
    Serial.print("\n");

    // decide item from condition
    condition.toLowerCase();
    if (condition.indexOf("rain") > -1) {
      Serial.print("Rain Jacket");
    }
    else if (condition.indexOf("sun") > -1) {
      Serial.print("Sunglasses");
    }
    else if (condition.indexOf("snow") > -1) {
      Serial.print("Skis");
    } else {
      Serial.print("None");
    }

    // mark item split
    Serial.print(";");

    // decide item from temperatures
    if (highTemp >= 20) {
      Serial.print("T-shirt");
    }
    else if (lowTemp < 10) {
      Serial.print("Warm Jacket");
    } else {
      Serial.print("Sweather");
    }
    
}

void wifiEnabled() {

  // check if connected to wifi
  if (WiFi.status() == WL_CONNECTED) {
    //Serial.println("Connected! D1 online...");

    // connect to api and get content
    HTTPClient httpClient;
    httpClient.begin(apiAUTHENTICATED);
    int httpReturnValue = httpClient.GET();

    // check if anything is being returned
    if (httpReturnValue > 0) {

      // get data as string
      //Serial.println("Getting payload...");
      String payload = httpClient.getString();

      // create jsonbuffer of sufficient size to store json object from string
      DynamicJsonBuffer jsonBuffer(20000);
      // create json object from string
      JsonObject& root = jsonBuffer.parseObject(payload, 50);

      // print if creation of json object fails
      if (!root.success()) {
        Serial.println("jsonBuffer.parseObject() unsuccessful;failed");
      }

      // store wanted values in appropriate variables
      int highTemp = root["forecast"]["simpleforecast"]["forecastday"][0]["high"]["celsius"];
      int lowTemp = root["forecast"]["simpleforecast"]["forecastday"][0]["low"]["celsius"];
      String condition = root["forecast"]["simpleforecast"]["forecastday"][0]["conditions"];

      // forecast item
      forecastItem(highTemp, lowTemp, condition);
      
    } else {
      // print if api was not found
      Serial.println("Something happened;Could not find API.");
    }
    // close http connection
    httpClient.end();
  }
  
}

void wifiDisabled() {
  
  // forecast from static values
  forecastItem(18,14, "Chance of Rain");
  //forecastItem(18,14, "Sunny");
  //forecastItem(20,15, "Cloudy");
  
}



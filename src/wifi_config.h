#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#elif defined(ESP32)
#endif

HTTPClient http;
WiFiClient wifiClient;

// Replace this with out own info:
const char *ssid = "";
const char *password = "";

// IP address where the home-hub-free server is running
String home_server = "http://192.168.1.72:8080";
// This id should be unique across devices
const uint32 id = ESP.getChipId();


void wifiConnect() {
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoReconnect(true);
	WiFi.begin(ssid, password);
	Serial.println("WiFi connecting...");
	while (!WiFi.isConnected()) {
		delay(100);
		Serial.print(".");
	}
	Serial.print("\n");
	Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
}

// Let the server know when motion is detected
void signalServer(int value) {
  Serial.print("Signaling server of:");
  Serial.println(value);
  http.begin(wifiClient, home_server + "/sensor-update");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST("{\"value\": " + String(value) + ", \"id\": \"" + String(id) + "\" }");
  if (httpCode > 0) {
    String result = http.getString();
    Serial.print("Result: ");
    Serial.println(result);
  }
  else {
    Serial.print("Oops: ");
    Serial.println(httpCode);
  }
  http.end();
}

// Once initialization is complete, let the server know we exisst
void declareSensor() {
  http.begin(wifiClient, home_server + "/sensor-declare");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST("{ \"id\": \"" + String(id) + "\", \"name\": \"motion\" }");
  if (httpCode > 0) {
    String result = http.getString();
    Serial.print("Result: ");
    Serial.println(result);
  }
  else {
    Serial.print("Oops: ");
    Serial.println(httpCode);
  }
  http.end();
}

#endif /* WIFI_INFO_H_ */
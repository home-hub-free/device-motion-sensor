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
String home_server = "";
// This id should be unique across devices
const uint32 chipId = system_get_chip_id();


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

void signalServer(int value) {
  Serial.print("Signaling server of:");
  Serial.println(value);
  http.begin(wifiClient, home_server + "/sensor-update");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST("{\"value\": " + String(value) + ", \"id\": " + String(chipId) + "}");
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

void ping() {
  http.begin(wifiClient, home_server + "/sensor-declare");
  http.addHeader("Content-Type", "application/json");
  String id = String(chipId);
  String jsonString = "{ \"id\": \"" + id + "\", \"name\": \"motion\" }";
  int httpCode = http.POST(jsonString);

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
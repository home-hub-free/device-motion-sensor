#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#elif defined(ESP32)
#endif

HTTPClient http;
WiFiClient wifiClient;
WiFiManager wifiManager;

// IP address where the home-hub-free server is running
String home_server = "http://192.168.1.199:8080";
// This id should be unique across devices
const uint32 chipId = system_get_chip_id();


void wifiConnect() {
  // Attemp to auto-connect for 10 seconds
  wifiManager.setConnectTimeout(10);
  wifiManager.setConfigPortalTimeout(300);
  wifiManager.autoConnect("HHF - Motion Sensor");
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
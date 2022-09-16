#include <Arduino.h>
#include <wifi_config.h>

int calibrationTime = 30; //the time when the sensor outputs a low impulse long unsigned 
bool state = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(D5, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(D5, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  wifiConnect();

  Serial.print("Calibrating sensor...");

  for (int i = 0; i < calibrationTime; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }

  ping();
}

void loop() {
  bool on = digitalRead(D5) == HIGH;
  if (state != on) {
    digitalWrite(LED_BUILTIN, on ? LOW : HIGH);
    state = on;
    Serial.print("State changed to: ");
    Serial.println(on);
    signalServer(on);
  }

  delay(50);
}
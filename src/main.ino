#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "config.h"
#include "sensors.h"
#include "telemetry.h"

// Objects
Sensors sensors;
Telemetry telemetry(sensors);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  if (!sensors.begin()) {
    Serial.println("INA219 not found!");
    while (1) { delay(10); }
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);
  Serial.println("System started. Waiting for data...");
}

void loop() {
  Blynk.run();
  static unsigned long lastMillis = 0;
  if (millis() - lastMillis > 2000) { // every 2 seconds
    lastMillis = millis();
    telemetry.send();
  }
}

#include "telemetry.h"
#include "config.h"
#include <BlynkSimpleEsp8266.h>
#include <Arduino.h>
#include <math.h>

Telemetry::Telemetry(Sensors &sensorRef)
  : sensors(sensorRef), overcurrentState(false), lowBatState(false), lastNoSunAlert(0) {}

int Telemetry::estimateBatteryPercent(float voltage) {
  if (voltage >= 4.2) return 100;
  if (voltage <= 3.2) return 0;
  return (int)((voltage - 3.2) * 100 / (4.2 - 3.2));
}

void Telemetry::handleNoSun(float busV, float avgCurr, float powerW) {
  if (powerW < NO_SUN_PWR_THRESHOLD && fabs(avgCurr) < 0.02) {
    unsigned long now = millis();
    if (now - lastNoSunAlert > NO_SUN_COOLDOWN) {
      lastNoSunAlert = now;
      Serial.println("⚠ No-sun detected: low solar power");
      Blynk.logEvent("no_charge", "No or very low solar power detected");
      digitalWrite(LED_PIN, HIGH); delay(200); digitalWrite(LED_PIN, LOW);
    }
  }
}

void Telemetry::send() {
  float busV = sensors.getBusVoltage();
  float shuntV = sensors.getShuntVoltage();
  float currA = sensors.getCurrent();

  // Rolling average
  sensors.addCurrentSample(currA);
  float avgCurr = sensors.getAvgCurrent();
  if (fabs(avgCurr) < NOISE_DEADBAND_A) avgCurr = 0.0;

  float powerW = busV * avgCurr;
  int batPercent = estimateBatteryPercent(busV);

  // Serial debug
  Serial.println("====================================");
  Serial.print("Bus Voltage     : "); Serial.print(busV,3); Serial.println(" V");
  Serial.print("Shunt Voltage   : "); Serial.print(shuntV,3); Serial.println(" mV");
  Serial.print("Current (avg)   : "); Serial.print(avgCurr,4); Serial.println(" A");
  Serial.print("Power           : "); Serial.print(powerW,4); Serial.println(" W");
  Serial.print("Battery Level   : "); Serial.print(batPercent); Serial.println(" %");

  // Send to Blynk
  Blynk.virtualWrite(V1, busV);
  Blynk.virtualWrite(V2, avgCurr);
  Blynk.virtualWrite(V3, powerW);
  Blynk.virtualWrite(V4, batPercent);

  // Overcurrent
  const float OVER_CURRENT_ON  = HIGH_CURRENT;
  const float OVER_CURRENT_OFF = HIGH_CURRENT - 0.2;
  if (!overcurrentState && avgCurr > OVER_CURRENT_ON) {
    overcurrentState = true;
    digitalWrite(LED_PIN, HIGH);
    Blynk.logEvent("overcurrent", "Overcurrent detected");
    Serial.println("⚠ ALERT: Overcurrent detected!");
  } else if (overcurrentState && avgCurr < OVER_CURRENT_OFF) {
    overcurrentState = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("✅ Overcurrent cleared");
  }

  // Low Battery
  const float LOW_ON  = LOW_VOLTAGE;
  const float LOW_OFF = LOW_VOLTAGE + 0.05;
  if (!lowBatState && busV < LOW_ON) {
    lowBatState = true;
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.logEvent("low_battery", "Battery voltage low");
    Serial.println("⚠ ALERT: Low Battery!");
  } else if (lowBatState && busV > LOW_OFF) {
    lowBatState = false;
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("✅ Battery voltage back to safe");
  }

  handleNoSun(busV, avgCurr, powerW);
  Serial.println("====================================\n");
}

#include "sensors.h"
#include "config.h"

Sensors::Sensors() : currIdx(0), windowFilled(false) {}

bool Sensors::begin() {
  return ina219.begin();
}

float Sensors::getBusVoltage() {
  return ina219.getBusVoltage_V();
}

float Sensors::getShuntVoltage() {
  return ina219.getShuntVoltage_mV();
}

float Sensors::getCurrent() {
  return ina219.getCurrent_mA() / 1000.0;
}

void Sensors::addCurrentSample(float val) {
  currWindow[currIdx++] = val;
  if (currIdx >= AVG_WINDOW) { currIdx = 0; windowFilled = true; }
}

float Sensors::getAvgCurrent() {
  int count = windowFilled ? AVG_WINDOW : currIdx;
  if (count == 0) return 0.0;
  float sum = 0;
  for (int i = 0; i < count; i++) sum += currWindow[i];
  return sum / count;
}

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "sensors.h"

class Telemetry {
public:
  Telemetry(Sensors &sensorRef);
  void send();
private:
  Sensors &sensors;
  bool overcurrentState;
  bool lowBatState;
  unsigned long lastNoSunAlert;

  int estimateBatteryPercent(float voltage);
  void handleNoSun(float busV, float avgCurr, float powerW);
};

#endif

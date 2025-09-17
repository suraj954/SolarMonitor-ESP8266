#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_INA219.h>

class Sensors {
public:
  Sensors();
  bool begin();
  float getBusVoltage();
  float getShuntVoltage();
  float getCurrent();      // Raw current in A
  void addCurrentSample(float val);
  float getAvgCurrent();

private:
  Adafruit_INA219 ina219;
  float currWindow[AVG_WINDOW];
  int currIdx;
  bool windowFilled;
};

#endif

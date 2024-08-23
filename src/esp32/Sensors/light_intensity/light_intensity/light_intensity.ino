#include <Wire.h>
#include "DFRobot_VEML7700.h"

DFRobot_VEML7700 als;
  float lux;
void setup() {
  Serial.begin(9600);
  als.begin();
}

void loop() {

  als.getALSLux(lux);
  Serial.print("Lux: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(200);
}


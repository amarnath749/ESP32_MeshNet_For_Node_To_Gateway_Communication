// Author : https://github.com/aruncs31s

#ifndef MYLIBRARY_H
#define MYLIBRARY_H
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

float getUltrasonicReadings(short trigPin, short echoPin) {
  int duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration * SOUND_SPEED / 2; // Return Distance in CentiMeter
}

#endif

#include "IMU.h"

Kompas kompas;

void setup() {
  Serial.begin(9600);
  kompas.init();
}

void loop() {
  kompas.printAccel();
}
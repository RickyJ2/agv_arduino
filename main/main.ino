#include "MotorDriver.h"
#include "Proximity.h"
#include "LimitSwitch.h"
#include "IMU.h"
#include "voltageReader.h"

MotorDriver car(6,7,8,9,10,11);
Proximity proximity(5);
LimitSwitch limitUpper(12);
LimitSwitch limitBellow(13);
Kompas kompas(5);
VoltageReader powerbank(A2,3.2, 5.1);
VoltageReader baterai(A3,4.8, 7.7);

void setup() {
  Serial.begin(9600);
  proximity.init();
  limitUpper.init();
  limitBellow.init();
  kompas.init();
}

void loop() {
  
}
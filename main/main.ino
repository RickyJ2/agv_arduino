#include "LimitSwitch.h"
#include "Proximity.h"

// LimitSwitch limitA(7);
Proximity proximity(2);

void setup() {
  Serial.begin(9600);
  proximity.init();
  // limitA.init();
  
}

void loop() {
  Serial.println(proximity.isObjectDetected());
}
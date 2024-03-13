#include "voltageReader.h"

VoltageReader v(A0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(v.getVolt());
  Serial.print("\t");
  Serial.println(v.getPercent());

}
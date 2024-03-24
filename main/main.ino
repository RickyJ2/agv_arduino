#include "MotorDriver.h"
#include "Proximity.h"
#include "LimitSwitch.h"
#include "IMU.h"
#include "voltageReader.h"

MotorDriver motor(6,7,8,9,10,11);
Proximity container(5);
LimitSwitch uppperBumper(12);
LimitSwitch bellowBumper(13);
Kompas imu(5);
VoltageReader powerbank(A2,3.2, 5.1);
VoltageReader battery(A3,4.8, 7.7);

void setup() {
  Serial.begin(9600);
  container.init();
  uppperBumper.init();
  bellowBumper.init();
  imu.init();
  powerbank.init();
  battery.init();
}

void loop() {
  //Update all sensor data
  unsigned long currentMillis = millis();
  container.updateState(currentMillis);
  uppperBumper.updateState(currentMillis);
  bellowBumper.updateState(currentMillis);
  imu.updateState();
  powerbank.updateState();
  battery.updateState();
  //Send to raspberry
  String msg =
    "{\"container\":" + String(container.getState()) + 
    ",\"collision\":" + String(uppperBumper.getState() || bellowBumper.getState()) + 
    ",\"orientation\":" + imu.getOrientationString() + 
    ",\"acceleration\":" + imu.getAccelerationString() + 
    ",\"power\":" + String(powerbank.getState() <= battery.getState() ? powerbank.getState() : battery.getState()) + //use lowest percent
  "}";
  Serial.println(msg);

  if(Serial.available() > 0){

  }
}
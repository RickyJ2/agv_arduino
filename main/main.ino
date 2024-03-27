#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
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
VoltageReader powerbank(A3, 4.8, 5.2);
VoltageReader battery(A2, 5, 7.7);

void setup() {
  motor.init();
  container.init();
  uppperBumper.init();
  bellowBumper.init();
  imu.init();
  powerbank.init();
  battery.init();
  Serial.begin(9600);
}

void loop() {
  //Update all sensor data
  unsigned long currentSecond = millis()/1000;
  container.updateState(currentSecond);
  uppperBumper.updateState();
  bellowBumper.updateState();
  imu.updateState();
  powerbank.updateState();
  battery.updateState();
  //Send to raspberry
  JsonDocument data;
  data["container"] = container.getState();
  data["collision"] = uppperBumper.getState() || bellowBumper.getState();
  vec3_t orientation = imu.getOrientation();
  vec3_t acceleration = imu.getAcceleration();
  data["orientation"]["yaw"] = orientation.x;
  data["orientation"]["roll"] = orientation.y;
  data["orientation"]["pitch"] = orientation.z;
  data["acceleration"]["x"] = acceleration.x;
  data["acceleration"]["y"] = acceleration.y;
  data["acceleration"]["z"] = acceleration.z;
  
  if(powerbank.getState() <= battery.getState()){
    data["power"] = powerbank.getState();
  }else{
    data["power"] = battery.getState();
  }
  serializeJson(data, Serial);
  Serial.println();

  if(Serial.available() > 0){
    JsonDocument input;
    deserializeJson(input, Serial);
    String cmd = input["cmd"];
    Serial.println("Received msg");
    //Collission Routine
    if(uppperBumper.getState() || bellowBumper.getState()){
      motor.stop();
    }else if(cmd == "forward"){
      Serial.println("FORWARD");
      motor.forward();
    }else if(cmd == "backward"){
      Serial.println("BACKWARD");
      motor.backward();
    }else if(cmd == "left"){
      Serial.println("LEFT");
      motor.turnLeft();
    }else if(cmd == "right"){
      Serial.println("RIGHT");
      motor.turnRight();
    }else if(cmd == "stop"){
      Serial.println("STOP");
      motor.stop();
    }
  }
}
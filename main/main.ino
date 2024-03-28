#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include "MotorDriver.h"
#include "Proximity.h"
#include "LimitSwitch.h"
#include "IMU.h"
#include "voltageReader.h"
#include "PIDController.h"
#define maxSpeed 255
#define minSpeed 160

MotorDriver motor(6,7,8,9,10,11);
Proximity container(5);
LimitSwitch uppperBumper(12);
LimitSwitch bellowBumper(13);
Kompas imu(5);
VoltageReader powerbank(A3, 4.8, 5.2);
VoltageReader battery(A2, 5, 7.7);
PIDController pid(2,5,1);
//AGV State
float targetAngle = 0;
bool isDriving = false;
bool direction = true; //true: forward, false: backward

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
    //Collission Routine
    // if(uppperBumper.getState() || bellowBumper.getState()){
    //   motor.stop();
    // }else 
    if(cmd == "forward"){
      motor.forward();
      isDriving = true;
      direction = true;
    }else if(cmd == "backward"){
      motor.backward();
      isDriving = true;
      direction = false;
    }else if(cmd == "left"){
      motor.turnLeft();
      targetAngle += 90;
      if (targetAngle > 180){
        targetAngle -= 360;
      }
      isDriving = false;
    }else if(cmd == "right"){
      motor.turnRight();
      targetAngle -= 90;
      if (targetAngle <= -180){
        targetAngle += 360;
      }
      isDriving = false;
    }else if(cmd == "stop"){
      motor.stop();
      isDriving = false;
    }
  }
  //PID Calculate
  // double output = pid.compute(orientation.x, targetAngle);
  // if(output > 0){

  // }
}
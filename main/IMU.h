#include <Wire.h>
#include <I2Cdev.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include <vector_type.h>

volatile bool mpuInterrupt = false;

static void dmpDataReady() {
  mpuInterrupt = true;
}

class Kompas{
  private:
    MPU6050 compass;
    int interrupt_pin;
    
    bool dmpReady = false;  // set true if DMP init was successful
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
    uint8_t mpuIntStatus; 
    uint8_t devStatus; 

    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorInt16 aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

  public:
    Kompas(int int_pin = 5){
      interrupt_pin = int_pin;
    }

    void init(){
      // Untuk menginisialisasi Kompas
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

      compass.initialize();
      pinMode(interrupt_pin, INPUT);

      //Verify connection
      testConnection();
      devStatus = compass.dmpInitialize();

      //Change with sensor's offset value
      compass.setXGyroOffset(121);
      compass.setYGyroOffset(-31);
      compass.setZGyroOffset(3);
      compass.setXAccelOffset(797);
      compass.setYAccelOffset(-79);
      compass.setZAccelOffset(201);
      
      if (devStatus == 0) {
        compass.CalibrateAccel(6);
        compass.CalibrateGyro(6);
        compass.PrintActiveOffsets();
        compass.setDMPEnabled(true);    
        attachInterrupt(digitalPinToInterrupt(interrupt_pin), dmpDataReady, RISING);
        mpuIntStatus = compass.getIntStatus();
        dmpReady = true;
        // get expected DMP packet size for later comparison
        packetSize = compass.dmpGetFIFOPacketSize();
      }
    }

    bool testConnection(){
      return compass.testConnection();
    }

    void updateState(){
      compass.dmpGetCurrentFIFOPacket(fifoBuffer);
      compass.dmpGetQuaternion(&q, fifoBuffer);
      compass.dmpGetGravity(&gravity, &q);
      compass.dmpGetAccel(&aa, fifoBuffer);
      compass.dmpGetYawPitchRoll(ypr, &q, &gravity);
      //Update Orientation value +180 s/d -180
      ypr[0] = (ypr[0] * 180 / M_PI); 
      ypr[1] = (ypr[1] * 180 / M_PI); // PITCH
      ypr[2] = (ypr[2] * 180 / M_PI); // ROLL
      //Update Acceleration value
      compass.dmpGetLinearAccel(&aaReal, &aa, &gravity);
    }

    vec3_t getOrientation(){
      return {ypr[0], ypr[1], ypr[2]};
    }

    vec3_t getAcceleration(){
      return {aaReal.x, aaReal.y, aaReal.z};
    }

    // String getOrientationString(){
    //   String data =  
    //     "{\"yaw\":" + String(ypr[0]) +
    //     ",\"pitch\":" + String(ypr[1]) +
    //     ",\"roll\":" + String(ypr[2]) +
    //     "}";
    //   return data;
    // }

    // String getAccelerationString(){
    //   String data =  
    //     "{\"x\":" + String(aaReal.x) +
    //     ",\"y\":" + String(aaReal.y) +
    //     ",\"z\":" + String(aaReal.z) +
    //     "}";
    //   return data;
    // }
};
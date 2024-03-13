#define INTERRUPT_PIN 8
#include <Wire.h>
#include <I2Cdev.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include <vector_type.h>

volatile bool mpuInterrupt = false;

void dmpDataReady() {
    mpuInterrupt = true;
}

class Kompas{
    private:
        MPU6050 compass;
        
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
        Kompas(){}

        void init(){
            // Untuk menginisialisasi Kompas
            #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
                Wire.begin();
                Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
            #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
                Fastwire::setup(400, true);
            #endif

            compass.initialize();
            pinMode(INTERRUPT_PIN, INPUT);

            // verify connection
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

                Serial.println(F("Enabling DMP..."));
                compass.setDMPEnabled(true);

                Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
                Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
                Serial.println(F(")..."));
                attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
                mpuIntStatus = compass.getIntStatus();

                Serial.println(F("DMP ready! Waiting for first interrupt..."));
                dmpReady = true;

                // get expected DMP packet size for later comparison
                packetSize = compass.dmpGetFIFOPacketSize();
            }
            else {
                // ERROR!
                // 1 = initial memory load failed
                // 2 = DMP configuration updates failed
                // (if it's going to break, usually the code will be 1)
                Serial.print(F("DMP Initialization failed (code "));
                Serial.print(devStatus);
                Serial.println(F(")"));
            }

        }

        bool testConnection(){
          // Test koneksi dari compass
          Serial.println("Testing device connections...");
          bool res = compass.testConnection();
          Serial.println(res ? "MPU6050 connection successful" : "MPU6050 connection failed");

          return res;
        }

        void printOrientation(){
          // Untuk mendapatkan nilai sudut dari kompas
          compass.dmpGetCurrentFIFOPacket(fifoBuffer);

          compass.dmpGetQuaternion(&q, fifoBuffer);
          compass.dmpGetGravity(&gravity, &q);
          compass.dmpGetYawPitchRoll(ypr, &q, &gravity);
          Serial.print("Yaw: ");
          Serial.print(ypr[0] * 180/M_PI + 180);
          Serial.print(",");
          Serial.print("Pitch: ");
          Serial.print(ypr[1]  * 180/M_PI);
          Serial.print(",");
          Serial.print("Roll: ");
          Serial.println(ypr[2] * 180/M_PI);
        }

        void printAccel(){
          compass.dmpGetCurrentFIFOPacket(fifoBuffer);

          compass.dmpGetQuaternion(&q, fifoBuffer);
          compass.dmpGetAccel(&aa, fifoBuffer);
          compass.dmpGetGravity(&gravity, &q);
          compass.dmpGetLinearAccel(&aaReal, &aa, &gravity);

          Serial.print("areal\t");
          Serial.print(aaReal.x);
          Serial.print("\t");
          Serial.print(aaReal.y);
          Serial.print("\t");
          Serial.println(aaReal.z);
        }

        vec3_t getOrientation(){
            compass.dmpGetCurrentFIFOPacket(fifoBuffer);

            compass.dmpGetQuaternion(&q, fifoBuffer);
            compass.dmpGetGravity(&gravity, &q);
            compass.dmpGetYawPitchRoll(ypr, &q, &gravity);

            // YAW dikonversi ke sistem +360
            ypr[0] = (ypr[0] * 180 / M_PI) + 180; 
            //pitch dan roll tetap basis +180 s/d -180
            ypr[1] = (ypr[1] * 180 / M_PI); // PITCH
            ypr[2] = (ypr[2] * 180 / M_PI); // ROLL

            return {ypr[0], ypr[1], ypr[2]};
        }
};
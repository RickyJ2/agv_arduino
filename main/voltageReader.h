#define VccMax 5
#define VccCorrection 1.017173 //getVolt()/Real Voltage
#include "math.h"

/*
  GND_Battery
      |---<10K Ohm>- A0 -<10K Ohm>- (+)Battery          
  GND_Arduino 
*/

class VoltageReader {
  private:
    int pin;
    float lowBat;
    float fullBat;
  
  public:
    // Constructor
    VoltageReader(int pinNumber, float lowBat, float fullBat) {
      pin = pinNumber;
      this-> lowBat = lowBat;
      this-> fullBat = fullBat;
    }
    
    float getVolt(){
      return analogRead(pin) * VccMax / 1023.0 * 2 * VccCorrection;
    }

    int getPercent(){
      return round((getVolt() - lowBat)/(fullBat - lowBat) * 100);
    }
};
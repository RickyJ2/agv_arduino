/*
  C - GND
  NO - not connected
  NC - digital Pin
*/

class LimitSwitch {
  private:
    int pin;
  
  public:
    // Constructor
    LimitSwitch(int pinNumber) {
      pin = pinNumber;
    }

    void init(){
      pinMode(pin, INPUT_PULLUP);
    }
    
    bool isPressed() {
      return digitalRead(pin) == HIGH;
    }
};
/*
  C - GND
  NO - not connected
  NC - digital Pin
*/

class LimitSwitch {
  private:
    int pin;
    bool state;
    unsigned long previousMillis = 0;
  
  public:
    LimitSwitch(int pinNumber) {
      pin = pinNumber;
      state = false;
    }

    void init(){
      pinMode(pin, INPUT_PULLUP);
      state = isPressed();
    }

    //update state after certain time
    void updateState(unsigned long current){
      if(current - previousMillis >= 2000){
        previousMillis = current;
        state = isPressed();
      }
    }

    bool getState(){
      return state;
    }
    
    bool isPressed() {
      return digitalRead(pin) == HIGH;
    }
};
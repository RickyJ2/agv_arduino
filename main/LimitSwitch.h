/*
  C - GND
  NO - not connected
  NC - digital Pin
*/

class LimitSwitch {
  private:
    int pin;
    bool state;
    unsigned long previousSecond = 0;
  
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
      if(current - previousSecond >= 1){
        previousSecond = current;
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
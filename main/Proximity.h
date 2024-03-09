//Don't forget configurate potentiometer on Sensor
class Proximity {
  private:
    int pin;
  
  public:
    // Constructor
    Proximity(int pinNumber) {
      pin = pinNumber;
    }

    void init(){
      pinMode(pin, INPUT);
    }
    
    bool isObjectDetected() {
      return digitalRead(pin) == LOW;
    }
};
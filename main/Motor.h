class Motor {
  private:
    int enabPin;
    int in1;
    int in2;

  public:
    Motor(){}
    Motor(int enabPin, int in1, int in2){
      this->enabPin = enabPin;
      this->in1 = in1;
      this->in2 = in2;
    }

    void init(){
      pinMode(enabPin, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
    }

    void setSpeed(int speed){
      analogWrite(enabPin, speed);
    }

    void forward(){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }

    void backward(){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    void stop(){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
};
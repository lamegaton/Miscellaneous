//Thanks 4CORE and Jmarlins for helping me.
//NodeMCU.h to map the pin on the motor shield correctly
/*
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
*/
#include "NodeMCU.h" 
#define TRIGGER PIN_D5
#define ECHO PIN_D7
// map the motor driver channels
#define PIN_R_PWM PIN_PWM_A // A is right
#define PIN_R_DIR PIN_DIR_A // High is forward

#define PIN_L_PWM PIN_PWM_B // B is left
#define PIN_L_DIR PIN_DIR_B // High is forward
//declare const
const int RUN = 0;
const int TURN = 1;
const int OB = 30;
 
// NodeMCU Pin D1 > TRIGGER | Pin D2 > ECHO
 
void setup() {
 
  Serial.begin (9600);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(PIN_L_PWM, OUTPUT);
  pinMode(PIN_L_DIR, OUTPUT);
  pinMode(PIN_R_PWM, OUTPUT);
  pinMode(PIN_R_DIR, OUTPUT);
}
 
void loop() {
  stateMachine();
}

long checkDistance(){
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); //default is 2s
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); //default is 10s
  digitalWrite(TRIGGER, LOW);
  
  duration = pulseIn(ECHO, HIGH);//read a pulse
  //Serial.println(duration);
  distance = (duration/2) / 29.1;
  
  Serial.print(distance);
  Serial.println(" Centimeter");
  delay(10);
  return distance;
}

void controlHardware(int left, int right){
  left = constrain(left, -1023,1023);
  right = constrain(right, -1023,1023);
  
  digitalWrite(PIN_L_DIR, left >=0);
  digitalWrite(PIN_R_DIR, right >=0);
  
  analogWrite(PIN_L_PWM,abs(left));
  analogWrite(PIN_R_PWM,abs(right));
}

void stateMachine(){
  static int state = RUN;
  static long distance;
  switch(state){
    case RUN:
      // run forward until sensor near object
      distance = checkDistance();
      do{
        controlHardware(1023,1023);//run forward
        distance = checkDistance();
      } while(distance > OB);
      state = TURN;
      break;
    case TURN:
      // Turn left to avoid obstacle
      distance = checkDistance();
      do{
        controlHardware(-1023,-1023);//run one motor
        delay(1000);
        controlHardware(-1023,1023);
        delay(800);
        distance = checkDistance();
      } while (distance < OB);
      state = RUN;
      break;
  }
}

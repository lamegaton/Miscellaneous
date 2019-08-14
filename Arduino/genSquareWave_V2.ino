/*
  PIN 11 for piezo
  PIN 10 DAQ
  PIN 9 CPA
  PIN 8 CPB

  1. AB: CPA sends a pulse first then CPB
  2. BA: CPB sends a pulse then CPA
  3. ALL: all send pulses at the same time.
*/
int ab,temp = 0;
int duration = 1000;
unsigned long halfPeriod = 5000UL; // 5mS = 1/2 100 Hz 
// don't use a floating # for delay, delayMicroseconds

void piezo(){
  digitalWrite(11, LOW);
  delay(20);
  for(int i = 0; i< 8; i++){
      PINB = PINB | 0b00001000; // x-x-13-12-11-10-9-8 on Uno, 
                            // toggle output by writing 1 to input register
      delayMicroseconds(halfPeriod);
    }  
}

void daq(){
//    digitalWrite(8, LOW);    
//    digitalWrite(9, LOW);
//    digitalWrite(10, LOW);
    PORTB &= 0b00100000;// this are equivalent to 3 lines above
                        // set port to LOW
    switch(ab){
    case(1):// this means CPA will send a pulse first then CPB
      Serial.println("AB mode");
      PINB = PINB | 0b00000110; //10 and 9 HIGH
      delayMicroseconds(1);
      PINB = PINB | 0b00000110; //10 and 9 LOW
      delayMicroseconds(200);
      PINB = PINB | 0b00000101; //10 and 8 HIGH
      delayMicroseconds(1);
      PINB = PINB | 0b00000101; //10 and 8 LOW
      delayMicroseconds(200);
      break;
    case(2):
      Serial.println("BA mode");
      PINB = PINB | 0b00000101; //10 and 8 HIGH
      delayMicroseconds(1);
      PINB = PINB | 0b00000101; //10 and 8 LOW
      delayMicroseconds(200);
      PINB = PINB | 0b00000110; //10 and 9 HIGH
      delayMicroseconds(1);
      PINB = PINB | 0b00000110; //10 and 9 LOW
      break;
    default:
      Serial.println("Sync mode");
      PINB = PINB | 0b00000111; //10 and 9 HIGH
      delayMicroseconds(1);
      PINB = PINB | 0b00000111; //10 and 9 LOW
      delayMicroseconds(200);
      break;
    }
}

void setup()
{
Serial.begin(9600); 
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);  
pinMode(10, OUTPUT);
pinMode(11, OUTPUT);
//digitalWrite (8, LOW); // known starting level
//digitalWrite (9, LOW); // known starting level
//digitalWrite (10, LOW); // known starting level
//digitalWrite (11, LOW); // known starting level
PORTB &= 0b00100000;
}

void loop(){
  if (Serial.available()){
    temp = Serial.read();
    if (temp != 10){
      ab = temp - 48;
//      //Burst Mode      
//      for (int i = 0; i < 10; i++){
//        daq();  
//      }
    }
  }
  daq();
}

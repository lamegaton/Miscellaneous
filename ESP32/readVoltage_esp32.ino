// use pin 36 to read voltage

int value = 0;
int maxValue = 0;
int minValue = 4096;

void calibration(){
  while(millis() < 3000){
    value = analogRead(36);
    if (value > maxValue){
      maxValue = value;
    }
    if (value < minValue){
      minValue = value;
    }
    delay(100);
  }
}

void val2Voltage(int *val){
  float voltage = *val * (3.333333 / 4096.000);
  Serial.println(voltage,6);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //analogSetWidth(12);
  analogReadResolution(12);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(34);
  val2Voltage(&value);
  //float voltage = value * (3.333333 / 4096.000);
  //Serial.println(voltage,6);
  delay(100);
}

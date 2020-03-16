/* 
03/03/2019
Use pin 2 to read voltage (5ft pin (right))
Code by Son Pham 
sonpham995@gmail.com
credits to: https://github.com/G6EJD/ESP32-ADC-Accuracy-Improvement-function/blob/master/ESP32_Read_Voltage_Simple.ino
I also use 270 ohm resistor to measure current.
*/

uint16_t resistor = 270; 
int value = 0;
int maxValue = 0;
int minValue = 4096;

float refVolt = 3.333333;
float voltage = 0;
float alpha = 0.7;
byte res = 12;
byte ADCpin = 2;

void calibration(){
  while(millis() < 3000){
    value = analogRead(ADCpin);
    if (value > maxValue){
      maxValue = value;
    }
    if (value < minValue){
      minValue = value;
    }
    delay(100);
  }
  Serial.print("max "); val2Voltage(&maxValue);
  Serial.print("min "); val2Voltage(&minValue);
}

void val2Voltage(int *val){
  voltage = (*val * (refVolt / (1 << res)));
  Serial.print("Voltage(V): ");Serial.print(voltage,6);Serial.print("     ");
}

void setup() {
  adcAttachPin(ADCpin);
  analogSetClockDiv(255); // 1338mS
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogSetWidth(12);
  analogReadResolution(12);
  //analogSetAttenuation(ADC_0db);
  calibration();
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(ADCpin);
  int val = alpha * value + (1-alpha) * val; //running average

  val2Voltage(&val);
  //float voltage = value * (3.333333 / 4096.000);
  //Serial.println(voltage,6);
  Serial.print("Current(mA): "); Serial.println(voltage*1000/resistor,3);
  delay(200);
}

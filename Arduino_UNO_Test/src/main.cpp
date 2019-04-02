#include <Arduino.h>
int analogPin = A0;
long val = 0;
float tempVal = 0;
char buffer[256];
void setup() {
  // 配置串口波特率
  Serial.begin(115200); 
  // put your setup code here, to run once:
  Serial.print("Hello World!\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  val  = analogRead(analogPin);  // read the input pin
  tempVal = (val * 5000) / 1023;
  val = tempVal;
  sprintf(buffer, "Current voltage = %ld.%03ld V\n", val / 1000, val % 1000);
  Serial.print(buffer);
  delay(1000);
  
}
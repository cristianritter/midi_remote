#include <Arduino.h>
#include <pinout.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(led_bt1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led_bt1, HIGH);
  delay(100);
  digitalWrite(led_bt1, LOW);
  delay(1000);
  
}
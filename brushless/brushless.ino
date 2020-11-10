#include <Servo.h>

Servo ESC;

int potValue;

void setup() {
  Serial.begin(9600);
  
  ESC.attach(9, 1000, 2000); 
}
void loop() {
  potValue = analogRead(A0);
  potValue = map(potValue, 0, 1023, 1000, 2000);
  ESC.write(potValue);

  Serial.println(potValue);
  
}

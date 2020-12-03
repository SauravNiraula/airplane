#include <Servo.h>
#include <Wire.h>

Servo Esc;
Servo Sc21;
Servo Sc22;
Servo Sc3;
Servo Sc4;

int sc21_val, sc22_val, sc3_val, sc4_val;
int16_t c1 = 0, c2 = 125, c3 = 125, c4 = 125;

byte byteC1 = 0xFC;
byte byteC2 = 0xFD;
byte byteC3 = 0xFE;
byte byteC4 = 0xFF;

int incomingSize = 3;

int speed = 1000;
long lastTime;
bool calibrated = false;
bool dataReceived = false;

//mpu
//int o_prop = 0, o_derv = 0, prev_error = 0, to_set = 0, output = 0;
//int16_t ax;
//float o_intg = 0;

void setup() {

  Esc.attach(3, 1000, 2000);
  Sc21.attach(5, 0, 180);
  Sc22.attach(6, 0, 180);
  Sc3.attach(10, 0, 180);
  Sc4.attach(11, 0, 180);
  Serial.begin(9600);
  
  Wire.begin(11);
  Wire.onReceive(dataAvailable);
}


void loop() {

  
  
  while (!calibrated) {
    lastTime = millis();
    if (lastTime < 2000) {
      Esc.write(2000);
//      Serial.print("Writing:- ");
//      Serial.println(2000);
      return;
    }
    else {
      Esc.write(1000);
//      Serial.print("Writing:- ");
//      Serial.println(1000);
      if (lastTime < 4000) {
        calibrated = true;
      }
      return;
    }
  }

  
//  output = pid(&to_set, ax, 1, 1, 0.001, &o_prop, &o_derv, &o_intg, &prev_error);
//  sc21_val = map(output, -100, 100, 0, 180);
//  sc22_val = 180 - sc21_val;

//  Serial.println(output);
  
  if (millis() - lastTime > 5000 && dataReceived && speed > 1000) {
    speed--;
    sc21_val = 90;
    sc22_val = 90;
    sc3_val = 90;
    sc4_val = 90;
    delay(20);
  }

//  Serial.println(speed);
  Esc.write(speed);
  Sc21.write(sc21_val);
  Sc22.write(sc22_val);
  Sc3.write(sc3_val);
  Sc4.write(sc4_val);
  
 
}

//int pid(int *to_set, int current, float Kp, float Kd, float Ki, int* o_prop, int* o_derv, float* o_intg, int* prev_error) {
//
//  int current_error = *to_set - current;
//  
//  *o_prop = constrain(Kp * (current_error), -100, 100);
//  *o_derv = constrain(Kd * (current_error - *prev_error), -100, 100);
//  *o_intg = constrain(*o_intg + Ki * current_error, -100, 100);
//
//  *prev_error = current_error;
//  output = constrain(*o_intg + *o_prop + *o_derv, -100, 100);
//  return output;
//}


void dataAvailable(int dataLen) {

  byte check;
  int data;
   
  if (dataLen == incomingSize) {
    check = Wire.read();
    data = Wire.read()<<8 | Wire.read() ;
  }

  switch (check) {
    case 0xFC:
      c1 = data;
      break;
    case 0xFD:
      c2 = data;
      break;
    case 0xFE:
      c3 = data;
      break;
    case 0xFF:
      c4 = data;
      break;
//    case 0xEF:
//      ax = data;
//      break;
    
  }
//  Serial.println(c3);

  speed = map(c1, 0, 255, 1000, 2000);
//  to_set = map(c2, 0, 255, -50, 50);
  sc22_val = map(c2, 0, 255, 0, 180);
  sc21_val = 180 - sc22_val;
  sc3_val = 180 - map(c3, 0, 255, 0, 180);
  sc4_val = 180 - map(c4, 0, 255, 0, 180);
  lastTime = millis();
  dataReceived = true;
  
}

#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Servo.h>

Servo Esc;

SoftwareSerial s(5,6);
StaticJsonDocument<100> buffer;

int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;

int speed = 1000;
long lastTime;
bool calibrated = false;

void setup() {

  Serial.begin(115200);
  s.begin(9600);

  Esc.attach(9, 1000, 2000);
}


void loop() {


  
//  while (!calibrated) {
//    lastTime = millis();
//    if (lastTime < 2000) {
//      Esc.write(2000);
//      Serial.print("Writing:- ");
//      Serial.println(2000);
//      return;
//    }
//    else {
//      Esc.write(1000);
////      Serial.print("Writing:- ");
//      Serial.println(1000);
//      if (lastTime < 4000) {
//        calibrated = true;
//      }
//      return;
//    }
//  }
//  
//
//  Esc.write(speed);
//  Serial.println(speed);

  if (s.available() > 0) {

     DeserializationError error = deserializeJson(buffer, s);

     if(!error) {

       serializeJsonPretty(buffer, Serial);
       c1 = constrain(buffer["c1"], 0, 300);
       c2 = constrain(buffer["c2"], 0, 300);
       c3 = constrain(buffer["c3"], 0, 300);
       c4 = constrain(buffer["c4"], 0, 300);

        speed = constrain(map(c1, 0, 300, 1000, 2000), 1000, 2000);
        
     }
    
  }
  
 
}

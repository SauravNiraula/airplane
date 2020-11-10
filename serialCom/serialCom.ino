#include <ArduinoJson.h>

StaticJsonDocument<100> buffer;

void setup() {
  Serial.begin(9600);

  
}

void loop() {
  
  
  buffer["c1"] = 200;
  buffer["c2"] = 150;
  buffer["c3"] = 200;
  buffer["c4"] = 150;
  
  serializeJson(buffer, Serial);
  delay(1000);
}

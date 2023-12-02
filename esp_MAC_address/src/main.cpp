#include <Arduino.h>

#include "WiFi.h"
 
void setup(){
  	Serial.begin(230400);
  	WiFi.mode(WIFI_MODE_STA);
}
 
void loop(){
  	Serial.println(WiFi.macAddress());
	delay(1000);
}
#include <Arduino.h>


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available()) {
    uint8_t buffer;
    int comms = Serial1.readBytes(&buffer, 1);
    Serial.println(buffer);
  }
}


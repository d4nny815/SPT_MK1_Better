#include <Arduino.h>
#define BAUD_RATE 9600 //

uint8_t transmit_data = 0x00;
uint8_t receive_data = 0x00;


unsigned long cur_time;
unsigned long prev_time_sent = millis();
unsigned long transmit_time = 50;

void setup() {
	Serial.begin(115200);
  	Serial1.begin(BAUD_RATE);
	Serial.println("POWER_ON");
}

void loop() {
	if (Serial1.available()){
		int parity = Serial1.readBytes(&receive_data, 1);
		Serial.println(receive_data);
  	}
}

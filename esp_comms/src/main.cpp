#include <Arduino.h>
#include <DigitalInput.h>

// INPUT PINS
//Digital IN
#define SW_IGN 23
#define SW_LAUNCH 22
#define SW_OXYGEN 21
#define SW_FUEL 19
#define BTN_LAUNCH 18
#define SW_KSI 5

#define BAUD_RATE 9600 //

// Instantiate every component in the system
DigitalInput  ign_sw(SW_IGN);
DigitalInput  launch_sw(SW_LAUNCH);
DigitalInput  oxygen_sw(SW_OXYGEN);
DigitalInput  fuel_sw(SW_FUEL);
DigitalInput  launch_btn(BTN_LAUNCH);
DigitalInput  ksi_sw(SW_KSI);


// b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0
// heartbeat | valid | sw_ign | sw_launch | sw_oxygen | sw_fuel | launch_btn | ksi
uint8_t prev_transmit_data = 0x00; 
uint8_t transmit_data = 0x00;
const u_int8_t bit_ksi = 1;
const u_int8_t bit_launch_btn = 1 << 1;
const u_int8_t bit_sw_fuel = 1 << 2;
const u_int8_t bit_sw_oxygen = 1 << 3;
const u_int8_t bit_sw_launch = 1 << 4;
const u_int8_t bit_sw_ign = 1 << 5;
const u_int8_t bit_valid = 1 << 6;
u_int8_t bit_heartbeat = 1 << 7;


unsigned long cur_time;
unsigned long prev_time_sent = millis();
unsigned long transmit_time = 50;

byte buffer[16];
String timeString;
uint64_t time_data = 0;
uint16_t ducer1_data = 0;
uint16_t ducer2_data = 0;
uint16_t ducer3_data = 0;
uint16_t loadcell_data = 0;
String received_data;

void setup() {
  	Serial.begin(115200);
  	Serial2.begin(BAUD_RATE);
	Serial.println("POWER_ON");
	transmit_data |= bit_valid;
  	transmit_data |= bit_heartbeat;
}

void loop() {
  	// read the KSI bit
	if (ksi_sw.read()) transmit_data = transmit_data|bit_ksi;
	else transmit_data = transmit_data & (bit_ksi ^ 0xff);

	// read the IGNITION bit
	if (launch_btn.read()) transmit_data = transmit_data|bit_launch_btn;
	else transmit_data = transmit_data & (bit_launch_btn ^ 0xff);

	// read the FUEL bit
	if (fuel_sw.read()) transmit_data = transmit_data|bit_sw_fuel;
	else transmit_data = transmit_data & (bit_sw_fuel ^ 0xff);

	// read the OXYGEN bit
	if (oxygen_sw.read()) transmit_data = transmit_data|bit_sw_oxygen;
	else transmit_data = transmit_data & (bit_sw_oxygen ^ 0xff);

	//read the LAUNCH bit
	if (launch_sw.read()) transmit_data = transmit_data|bit_sw_launch;
	else transmit_data = transmit_data & (bit_sw_launch ^ 0xff);

	//read the IGNITION bit
	if (ign_sw.read()) transmit_data = transmit_data|bit_sw_ign;
	else transmit_data = transmit_data & (bit_sw_ign ^ 0xff);

  	cur_time = millis();
  	if (cur_time - prev_time_sent >= transmit_time) {
  	  	prev_time_sent = cur_time;
		if (transmit_data != prev_transmit_data) {
			prev_transmit_data = transmit_data;
			// Serial.println(transmit_data, BIN);
			Serial2.write(transmit_data);
		}

  	}

	if (Serial2.available()){
  	    Serial2.readBytes(buffer, 16);

  	    for (int i=0; i<8; i++) {
  	      	time_data |= buffer[i] << (56 - (i * 8));
  	    }

  	  	ducer1_data = buffer[8] << 8 | buffer[9];
  	  	ducer2_data = buffer[10] << 8 | buffer[11];
  	  	ducer3_data = buffer[12] << 8 | buffer[13];
  	  	loadcell_data = buffer[14] << 8 | buffer[15];


  	  	received_data = String(time_data) + "," + String(ducer1_data) + "," 
  	                + String(ducer2_data) + "," + String(ducer3_data) + ","
  	                 + String(loadcell_data);
  	  	Serial.println(received_data);

  	  	time_data = ducer1_data = ducer2_data = ducer3_data = loadcell_data = 0;
  	}

}

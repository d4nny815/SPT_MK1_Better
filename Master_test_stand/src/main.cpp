#include <Arduino.h>
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "SolenoidValve.h"
#include "Transducer.h"
#include "LoadCell.h"
#include "Thermoresistor.h"
#include "DataLogger.h"

#include <WiFi.h>
#include <esp_now.h>

// Analog IN
#define LOADCELL_PIN 19
#define TRANSDUCER1_PIN 18
#define TRANSDUCER2_PIN 17
#define TRANSDUCER3_PIN 16
#define TRANSDUCER4_PIN 15
#define TRANSDUCER5_PIN 14
#define TRANSDUCER6_PIN 13
#define THERMORESISTOR1_PIN 12
#define THERMORESISTOR2_PIN 11
#define THERMORESISTOR3_PIN 10
// Digital IN
#define ESTOP_PIN 32

// OUTPUT PINS
#define STOPLIGHT_GREEN_PIN 2
#define STOPLIGHT_YELLOW_PIN 3
#define STOPLIGHT_RED_PIN 4
#define OXYGEN_VALVE_PIN 7
#define FUEL_VALUE_PIN 8
#define IGN_PIN 9
#define out1_pin 30
#define out2_pin 31

#define random_pin 33

// Define the states of the system
enum testStandState {
  POWER_ON,
  KSI,
  LAUNCH,
  ESTOP, 
  TEST
};
testStandState STAND_STATE;
bool first_time_in_state = true;

// Instantiate every component in the system
SerialData serial_data;
// DigitalOutput red_light(STOPLIGHT_RED_PIN);  // lights are active low
// DigitalOutput yellow_light(STOPLIGHT_YELLOW_PIN);  // lights are active low
// DigitalOutput green_light(STOPLIGHT_GREEN_PIN);  // lights are active low
// DigitalOutput ign_wire(IGN_PIN);
// DigitalOutput out1(out1_pin);
// DigitalOutput out2(out2_pin);
// SoleinoidValve oxygen_valve(OXYGEN_VALVE_PIN, 100, 90);
// SoleinoidValve fuel_valve(FUEL_VALUE_PIN, 100, 90);
// Loadcell loadcell(LOADCELL_PIN, 50);
// DigitalInput estop(ESTOP_PIN);
// Transducer ducer1(TRANSDUCER1_PIN);
// Transducer ducer2(TRANSDUCER2_PIN);
// Transducer ducer3(TRANSDUCER3_PIN);
// Transducer ducer4(TRANSDUCER4_PIN);
// Transducer ducer5(TRANSDUCER5_PIN);
// Transducer ducer6(TRANSDUCER6_PIN);
// Thermoresistor thermoresistor1(THERMORESISTOR1_PIN);
// Thermoresistor thermoresistor2(THERMORESISTOR2_PIN);
// Thermoresistor thermoresistor3(THERMORESISTOR3_PIN);
DigitalOutput red_light(random_pin);  // lights are active low
DigitalOutput yellow_light(random_pin);  // lights are active low
DigitalOutput green_light(random_pin);  // lights are active low
DigitalOutput ign_wire(random_pin);
DigitalOutput out1(random_pin);
DigitalOutput out2(random_pin);
SoleinoidValve oxygen_valve(random_pin, 100, 90);
SoleinoidValve fuel_valve(random_pin, 100, 90);
Loadcell loadcell(random_pin, 50);
DigitalInput estop(random_pin);
Transducer ducer1(random_pin);
Transducer ducer2(random_pin);
Transducer ducer3(random_pin);
Transducer ducer4(random_pin);
Transducer ducer5(random_pin);
Transducer ducer6(random_pin);
Thermoresistor thermoresistor1(random_pin);
Thermoresistor thermoresistor2(random_pin);
Thermoresistor thermoresistor3(random_pin);
Transducer ducer_arr[] = {ducer1, ducer2, ducer3, ducer4, ducer5, ducer6};
Thermoresistor thermoresistor_arr[] = {thermoresistor1, thermoresistor2, thermoresistor3};

// b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0
// heartbeat | valid | sw_ign | sw_launch | sw_oxygen | sw_fuel | launch_btn | ksi 
int comms_status;
uint8_t comms = 0x00;
const u_int8_t bit_ksi = 1 << 0;
const u_int8_t bit_launch_btn = 1 << 1;
const u_int8_t bit_sw_fuel = 1 << 2;
const u_int8_t bit_sw_oxygen = 1 << 3;
const u_int8_t bit_sw_launch = 1 << 4;
const u_int8_t bit_sw_ign = 1 << 5;
const u_int8_t bit_valid = 1 << 6;
u_int8_t bit_heartbeat = 1 << 7;

esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0x40, 0x22, 0xd8, 0x3c, 0x37, 0xfc};
SerialData::dataPacket incomingPacket;
esp_err_t result;
String success;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  	// Serial.print("\r\nLast Packet Send Status:\t");
  	// Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  	memcpy(&incomingPacket, incomingData, sizeof(incomingPacket));
  	// Serial.print("Bytes received: ");
  	// Serial.println(len);
  	comms = incomingPacket.button_data;
}


void setup() {
	Serial.begin(115200);
	serial_data.set_start_time();

  	WiFi.mode(WIFI_STA);
	esp_now_init();
	esp_now_register_send_cb(OnDataSent);
	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  	peerInfo.channel = 0;  
  	peerInfo.encrypt = false;
	esp_now_add_peer(&peerInfo);
	esp_now_register_recv_cb(OnDataRecv);

	STAND_STATE = POWER_ON; 
	// STATE = TEST;
}

/************************************************
 * HERE STARTS THE REAL CODE
 *  /\/\/\                            /  \
 * | \  / |                         /      \
 * |  \/  |                       /          \
 * |  /\  |----------------------|     /\     |
 * | /  \ |                      |    /  \    |
 * |/    \|                      |   /    \   |
 * |\    /|                      |  | (  ) |  |
 * | \  / |                      |  | (  ) |  |
 * |  \/  |                 /\   |  |      |  |   /\
 * |  /\  |                /  \  |  |      |  |  /  \
 * | /  \ |               |----| |  |      |  | |----|
 * |/    \|---------------|    | | /|   .  |\ | |    |
 * |\    /|               |    | /  |   .  |  \ |    |
 * | \  / |               |    /    |   .  |    \    |
 * |  \/  |               |  /      |   .  |      \  |
 * |  /\  |---------------|/        |   .  |        \|
 * | /  \ |              /   NASA   |   .  |  NASA    \
 * |/    \|              (          |      |           )
 * |/\/\/\|               |    | |--|      |--| |    |
 * ------------------------/  \-----/  \/  \-----/  \--------
 *                         \\//     \\//\\//     \\//
 *                          \/       \/  \/       \/
 ************************************************
*/

void power_on_state () {
	if (first_time_in_state) {
		// Serial.println("POWERON state entry");
		ign_wire.turn_off();
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
		green_light.turn_off();
		yellow_light.turn_on();
		red_light.turn_on();
		first_time_in_state = false;
	}
	return;
};


void ksi_state () {
	if (first_time_in_state) {
		// Serial.println("KSI state entry");
		ign_wire.turn_off();
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
		green_light.turn_on();
		yellow_light.turn_off();
		red_light.turn_on();
		first_time_in_state = false;
	}

	if ((comms & bit_sw_fuel) && (comms & bit_sw_oxygen)) {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
	}
	else if ((comms & bit_sw_fuel) && !(comms & bit_sw_oxygen)) {
		fuel_valve.turn_on();
		oxygen_valve.turn_off();
	}
	else if ((comms & bit_sw_oxygen) && !(comms & bit_sw_fuel)) {
		oxygen_valve.turn_on();
		fuel_valve.turn_off();
	} 
	else {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
	}
	return;
};

void launch_state () {
	if (first_time_in_state) {
		// Serial.println("LAUNCH state entry");
		ign_wire.turn_off();
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
		green_light.turn_on();
		yellow_light.turn_on();
		red_light.turn_off();
		first_time_in_state = false;
	}
	
	if ((comms & bit_sw_launch)) {
		oxygen_valve.turn_on(); 
		fuel_valve.turn_on();
	}
	else {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
	}

	if ((comms & bit_sw_ign)) ign_wire.turn_on();	
	else ign_wire.turn_off();

	return;
};

void fail_state () {
	green_light.turn_off();
	yellow_light.turn_off();
	red_light.turn_off();

	ign_wire.turn_off();
	oxygen_valve.turn_off();
	fuel_valve.turn_off();
	return;
};

void test_state() {
	return;
}


void loop() {
	// if (!(digitalRead(ESTOP_PIN))) {
	// 	STAND_STATE = ESTOP;
	// }

	if (Serial.available()) {
		comms_status = Serial.readBytes(&comms, 1);
		if (comms == -1) { STAND_STATE = ESTOP; } // failed reading serial transmission
		if (!(comms & bit_valid)) { STAND_STATE = ESTOP; } // valid bit wasn't sent through
		Serial.print("from comms: ");
		Serial.println(comms, BIN);
	}

	Serial.println(comms, BIN);

	serial_data.accummulate_data(ducer_arr, thermoresistor_arr, loadcell);
	// serial_data.send_serial_data();
	serial_data.print_serial_data();

	if (serial_data.buffer_index == 7) {
		serial_data.buffer_index = 0;
		result = esp_now_send(broadcastAddress, (uint8_t *) &serial_data.outgoingDataPacket, sizeof(serial_data.outgoingDataPacket));
	}
	else {
		serial_data.buffer_index++;
	}

	if (result == ESP_OK) {
    	Serial.println("Sent with success");
  	}
  	else {
  	  	Serial.println("Error sending the data");
  	}

	switch (STAND_STATE) {
		case (POWER_ON): 
			power_on_state();

			if (comms & bit_ksi) {
				STAND_STATE = KSI;
				first_time_in_state = true;
			}
			break;

		case (KSI): 
			ksi_state();

			if (comms & bit_launch_btn) {
				STAND_STATE = LAUNCH;
				first_time_in_state = true;
			}
			if (!(comms & bit_ksi)) {
				STAND_STATE = POWER_ON;
				first_time_in_state = true;
			}
			break;

		case (LAUNCH): 
			launch_state();

			if (!(comms & bit_ksi)) {
				STAND_STATE = POWER_ON;
				first_time_in_state = true;
			}
			break;

		case (ESTOP): 
			fail_state();

			if (comms == 0xC0 && digitalRead(ESTOP_PIN)) {
				STAND_STATE = POWER_ON;
				first_time_in_state = true;
				green_light.turn_off();
				yellow_light.turn_off();
				red_light.turn_off();
			}
			break;

		case (TEST):
			test_state();
			break;
		default:
			STAND_STATE = ESTOP;
	}
}
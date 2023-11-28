#include <Arduino.h>
#include <stdlib.h>
#include <avr/io.h>
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "SolenoidValve.h"
#include "Transducer.h"
#include "LoadCell.h"
#include "DataLogger.h"


// INPUT PINS
#define BAUD_RATE 9600  // BAUDRATE for teensy communications
// Analog IN
#define TRANSDUCER1_PIN 18
#define TRANSDUCER2_PIN 17
#define TRANSDUCER3_PIN 16
#define LOADCELL_PIN 19
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

// Define the states of the system
enum State {
  POWER_ON,
  KSI,
  LAUNCH,
  FAIL, 
  TEST
};
State STATE;
bool first_time_in_state = true;

// Instantiate every component in the system
SerialData serial_data;
DigitalOutput red_light(STOPLIGHT_RED_PIN);  // lights are active low
DigitalOutput yellow_light(STOPLIGHT_YELLOW_PIN);  // lights are active low
DigitalOutput green_light(STOPLIGHT_GREEN_PIN);  // lights are active low
DigitalOutput ign_wire(IGN_PIN);
DigitalOutput out1(out1_pin);
DigitalOutput out2(out2_pin);
SoleinoidValve oxygen_valve(OXYGEN_VALVE_PIN, 100, 90);
SoleinoidValve fuel_valve(FUEL_VALUE_PIN, 100, 90);
Loadcell loadcell(LOADCELL_PIN, 50);
DigitalInput estop(ESTOP_PIN);
Transducer ducer1(TRANSDUCER1_PIN);
Transducer ducer2(TRANSDUCER2_PIN);
Transducer ducer3(TRANSDUCER3_PIN);
Transducer ducer_arr[] = {ducer1, ducer2, ducer3};


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

void setup() {
	Serial.begin(115200);
	Serial1.begin(BAUD_RATE);
	serial_data.set_start_time();
	STATE = POWER_ON; 

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
		// * comment this to remove it
		delay(100); // shitty blocking delay but it'll do 
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

	// Serial.println("FAIL");

	ign_wire.turn_off();
	oxygen_valve.turn_off();
	fuel_valve.turn_off();
	return;
};

void test_state() {
	return;
}

void loop() {
	if (!(digitalRead(ESTOP_PIN))) {
		STATE = FAIL;
	}

	if (Serial1.available()) {
		comms_status = Serial1.readBytes(&comms, 1);
		
		if (comms == -1) { STATE = FAIL; } // failed reading serial transmission
		if (!(comms & bit_valid)) { STATE = FAIL; } // valid bit wasn't sent through
		// Serial.print("from comms: ");
		// Serial.println(comms, BIN);
	}

	// if (Serial.available()) {
	// 	comms_status = Serial.readBytes(&comms, 1);
	// 	if (comms == -1) { STATE = FAIL; } // failed reading serial transmission
	// 	if (!(comms & bit_valid)) { STATE = FAIL; } // valid bit wasn't sent through
	// 	Serial.print("from comms: ");
	// 	Serial.println(comms, BIN);
	// }

	serial_data.accummulate_data(ducer_arr, loadcell);
	serial_data.send_serial_data();
	// serial_data.print_serial_data();

	switch (STATE) {
		case (POWER_ON): 
			power_on_state();

			if (comms & bit_ksi) {
				STATE = KSI;
				first_time_in_state = true;
			}
			break;

		case (KSI): 
			ksi_state();

			if (comms & bit_launch_btn) {
				STATE = LAUNCH;
				first_time_in_state = true;
			}
			if (!(comms & bit_ksi)) {
				STATE = POWER_ON;
				first_time_in_state = true;
			}
			break;

		case (LAUNCH): 
			launch_state();

			if (!(comms & bit_ksi)) {
				STATE = POWER_ON;
				first_time_in_state = true;
			}
			break;

		case (FAIL): 
			fail_state();

			if (comms == 0xC0 && digitalRead(ESTOP_PIN)) {
				STATE = POWER_ON;
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
			STATE = FAIL;
	}
}
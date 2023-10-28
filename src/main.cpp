#include <Arduino.h>
#include <stdlib.h>
#include <avr/io.h>
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "Transducer.h"
// #include "LoadCell.h"
#include "DataLogger.h"


// INPUT PINS
// Analog IN
#define TRANSDUCER1_PIN 14
#define TRANSDUCER2_PIN 15
#define TRANSDUCER3_PIN 16
#define LOADCELL_PIN 17
// Digital IN
#define SW_LAUNCH_PIN 29 // both oxygen and fuel
#define SW_FUEL_VALUE_PIN 30
#define SW_OXYGEN_VALUE_PIN 31
#define ESTOP_PIN 32
#define KEY_IN_PIN 33
#define KEY_TURNED_PIN 34
#define SW_IGNITION 35

// OUTPUT PINS
#define STOPLIGHT_GREEN_PIN 5
#define STOPLIGHT_YELLOW_PIN 6
#define STOPLIGHT_RED_PIN 7
#define VALUE2_PIN 8
#define VALVE1_PIN 9
#define IGN_PIN 10


// Instantiate every component in the system
SerialData serial_data;
DigitalOutput red_light(STOPLIGHT_RED_PIN);
DigitalOutput yellow_light(STOPLIGHT_YELLOW_PIN);
DigitalOutput green_light(STOPLIGHT_GREEN_PIN);
DigitalOutput valve1(VALVE1_PIN);
DigitalOutput valve2(VALUE2_PIN);
DigitalOutput ign_wire(IGN_PIN);
DigitalInput key_in(KEY_IN_PIN);
DigitalInput key_turned(KEY_TURNED_PIN);
DigitalInput sw_fuel(SW_FUEL_VALUE_PIN);
DigitalInput sw_oxygen(SW_OXYGEN_VALUE_PIN);
DigitalInput sw_launch(SW_LAUNCH_PIN);
DigitalInput sw_ignition(SW_IGNITION);
Transducer ducer1(TRANSDUCER1_PIN);
Transducer ducer2(TRANSDUCER2_PIN);
Transducer ducer3(TRANSDUCER3_PIN);
Transducer ducer_arr[] = {ducer1, ducer2, ducer3};

// Define the states of the system
enum State {
  POWER_ON,
  KEY_IN,
  KEY_TURNED,
  FAIL,
  TEST
};
State STATE;
bool ESTOP = false;
bool first_time_in_state = true;


void setup() {
	Serial.begin(115200);
	Serial.println("POWER_ON");


	serial_data.print_header();
	STATE = POWER_ON;  // <- uncomment this line to start in POWER_ON state
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
		valve1.turn_off();
		valve2.turn_off();
		green_light.turn_on();
		yellow_light.turn_off();
		red_light.turn_off();
		first_time_in_state = false;
	}

	serial_data.accummulate_data(ducer_arr);
	// serial_data.print_serial_data();
	return;
};


void key_in_state () {
	if (first_time_in_state) {
		valve1.turn_off();
		valve2.turn_off();
		green_light.turn_off();
		yellow_light.turn_on();
		red_light.turn_off();
		first_time_in_state = false;
	}

	if (sw_fuel.read() & sw_oxygen.read()) {
		valve1.turn_off();
		valve2.turn_off();
	}
	else if (sw_fuel.read() & !sw_oxygen.read()) {
		valve1.turn_on();
		valve2.turn_off();
	}
	else if (sw_oxygen.read() & !sw_fuel.read()) {
		valve2.turn_on();
		valve1.turn_off();
	}

	serial_data.accummulate_data(ducer_arr);
	// serial_data.print_serial_data();
	return;
};

// TODO: for nick
void key_turned_state () {
	if (first_time_in_state) {
		valve1.turn_off();
		valve2.turn_off();
		green_light.turn_off();
		yellow_light.turn_off();
		red_light.turn_on();
		first_time_in_state = false;
	}
	
	//activates both valves at the same time 
	if(sw_launch.read()) {
		valve1.turn_on();
		valve2.turn_on();
	}
	else {
		valve1.turn_off();
		valve2.turn_off();
	}

	if(sw_ignition.read()){
		ign_wire.turn_on();
	}	
	else {
		ign_wire.turn_off();
	}
	serial_data.accummulate_data(ducer_arr);
	// serial_data.print_serial_data();
	return;
};

// TODO: for nick
// anything else for this state
void fail_state () {
	green_light.turn_on();
	yellow_light.turn_on();
	red_light.turn_on();

	ign_wire.turn_off();
	valve1.turn_off();
	valve2.turn_off();
	serial_data.accummulate_data(ducer_arr);
};

void loop() {
	// if (digitalRead(ESTOP_PIN) == HIGH) { // TODO: make this an ISR
	// 	STATE = FAIL;
	// }
	switch (STATE) {
		case (POWER_ON): 
			power_on_state();
			if (key_in.read()) {
				STATE = KEY_IN;
				first_time_in_state = true;
				Serial.println("going to KEY_IN from POWER_ON");
			}
			break;

		case (KEY_IN): 
			key_in_state();
			if (key_turned.read()) {
				STATE = KEY_TURNED;
				first_time_in_state = true;
				Serial.println("going to KEY_TURNED from KEY_IN");
			}
			if (!key_in.read()) {
				Serial.println(key_in.read());
				STATE = POWER_ON;
				first_time_in_state = true;
				Serial.println("going to POWER_ON from KEY_IN");
			}
			break;

		case (KEY_TURNED): 
			key_turned_state();
			break;

		case (FAIL): 
			fail_state();
			break;

		default:
			STATE = FAIL;  // something went wrong
	}
}
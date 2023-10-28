#include <Arduino.h>
#include <stdlib.h>
#include <avr/io.h>
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "Transducer.h"
// #include "LoadCell.h"
#include "DataLogger.h"


// INPUT PINS

// OUTPUT PINS
#define STOPLIGHT_GREEN_PIN 5
#define STOPLIGHT_YELLOW_PIN 6
#define STOPLIGHT_RED_PIN 7
#define VALUE2_PIN 8
#define VALVE1_PIN 9


// Instantiate every component in the system



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

// void accumulate_data() {
// 	serial_data.time = micros() - START_TIME_US;
// 	serial_data.tranducer1_data = ducer1.get_value_psi();
// 	serial_data.tranducer2_data = ducer2.get_value_psi();
// 	serial_data.tranducer3_data = ducer3.get_value_psi();
// 	// serial_data.loadcell_data = loadcell.read();

// 	return;
// };

int test_var;
void accumulate_data() {
	test_var++;
	serial_data.time = rand();
	serial_data.tranducer1_data = 100 * sin((PI / 180.0) * test_var);
	serial_data.tranducer2_data = 100 * cos((PI / 180.0) * test_var);
	serial_data.tranducer3_data = test_var;
	// serial_data.loadcell_data = loadcell.read();

	return;
}


u_int64_t START_TIME_US;
void setup() {
	Serial.begin(115200);
	Serial.println("POWER_ON");
	// STATE = POWER_ON;  // <- uncomment this line to start in POWER_ON state
	print_header();
	START_TIME_US = micros();
	STATE = TEST; // <- For testing purposes
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
		red_light.turn_off();
		yellow_light.turn_off();
		green_light.turn_on();
		first_time_in_state = false;
	}
	// TODO: everything that runs continuously in the POWER_ON state
	accumulate_data();
	print_serial_data(&serial_data);

	return;
};

//Turns on the Yellow Light and allows the valves to actuate
void key_in_state () {

	red_light.turn_off();
	yellow_light.turn_on();
	green_light.turn_off();

};

//Turns on Red Light, allows Launch Switch, Allows Ignition
void key_turned_state () {

	red_light.turn_on();
	yellow_light.turn_off();
	green_light.turn_off();


void fail_state () {};

void test_state() {
	// testing serial plotter
	Serial.println("i,math_func");
	for (u_int32_t i=0; i<0xffffffff; i++) {
		serial_data.tranducer1_data = i;
		serial_data.tranducer2_data = i - 500;
		serial_data.tranducer3_data = 1000 * sin(i * (PI / 180));
		serial_data.loadcell_data = 1000 * cos(i * (PI / 180));
		print_serial_data(&serial_data);		
		delay(10);
	}
};

void loop() {

	//Transducer ducer1; //Testing Transducer Lib
	//ducer1.voltageToPSI(4);

	if (digitalRead(ESTOP_PIN) == LOW) { // TODO: make this an ISR
		STATE = FAIL;
	}
	switch (STATE) {
		case (POWER_ON): 
			// TODO: POWER_ON state
			power_on_state();
			break;

		case (KEY_IN): 
			// TODO: KEY_IN state
			key_in_state();
			break;

		case (KEY_TURNED): 
			// TODO: KEY_TURNED state
			key_turned_state();
			break;

		case (FAIL): 
			// TODO: FAIL state
			fail_state();
			break;

		// TODO: remove this state
		// for testing purposes only
		case (TEST): 
			test_state();
			STATE = FAIL;
			break;

		default:
			STATE = FAIL;  // something went wrong
	}
}
#include <Arduino.h>
#include <stdlib.h>
#include <avr/io.h>
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "Transducer.h"
#include "LoadCell.h"
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

// OUTPUT PINS
#define STOPLIGHT_GREEN_PIN 5
#define STOPLIGHT_YELLOW_PIN 6
#define STOPLIGHT_RED_PIN 7
#define VALUE2_PIN 8
#define VALVE1_PIN 9


// Instantiate every component in the system
struct SerialData serial_data = {0, 0, 0, 0, 0};
DigitalOutput red_light(STOPLIGHT_RED_PIN);
DigitalOutput yellow_light(STOPLIGHT_YELLOW_PIN);
DigitalOutput green_light(STOPLIGHT_GREEN_PIN);
DigitalOutput valve1(VALVE1_PIN);
DigitalOutput valve2(VALUE2_PIN);


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


u_int64_t START_TIME_US;
void setup() {
	Serial.begin(115200);
	Serial.println("POWER_ON");
	// Serial.println("Time(us),Ducer1(psi), Ducer2(psi), Ducer3(psi), Loadcell(lbs)");
	// print_serial_data();
	// STATE = POWER_ON;  // <- uncomment this line to start in POWER_ON state
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
		red_light.turn_on();
		yellow_light.turn_off();
		green_light.turn_off();
		first_time_in_state = false;
	}
	// TODO: everything that runs continuously in the POWER_ON state
	
};


void key_in_state () {};

void key_turned_state () {};

void fail_state () {};

void test_state() {
	// testing serial plotter
	Serial.println("i,math_func");
	for (u_int32_t i=0; i<0xffffffff; i++) {
		serial_data.tranducer1_data = i;
		serial_data.tranducer2_data = i - 500;
		serial_data.tranducer3_data = 1000 * sin(i * (PI / 180));
		serial_data.loadcell_data = 1000 * cos(i * (PI / 180));
		print_serial_data();		
		delay(10);
	}
};

void loop() {
	if (digitalRead(ESTOP_PIN) == HIGH) { // TODO: make this an ISR
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
			Serial.println("IN TEST");
			test_state();
			STATE = FAIL;
			break;

		default:
			STATE = FAIL;  // something went wrong
	}
}
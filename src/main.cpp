#include <Arduino.h>
#include <stdlib.h>
#include <avr/io.h>
#include "SimpleIO.h"
#include "Transducer.h"
#include "LoadCell.h" // TODO: might need to be updated

// INPUT PINS
// TODO: change for proper pin numbers and names
#define TRANSDUCER1_PIN 10
#define TRANSDUCER2_PIN 11
#define TRANSDUCER3_PIN 12
#define LOADCELL_PIN 13
#define ESTOP_PIN 14
#define KEY_IN_PIN 15
#define KEY_TURNED_PIN 16
#define OXYGEN_VALUE_PIN 17
#define FUEL_VALUE_PIN 18
#define LAUNCH_PIN 19
#define IGNITION_PIN 34

// OUTPUT PINS
// TODO: change for proper pin numbers and names
#define VALVE1_PIN 20
#define VALUE2_PIN 21
#define DISPLAY_PIN 23
#define STOPLIGHT_RED_PIN 24
#define STOPLIGHT_YELLOW_PIN 25
#define STOPLIGHT_GREEN_PIN 26


// Instantiate every component in the system
SimpleIO red_light(STOPLIGHT_RED_PIN);
SimpleIO yellow_light(STOPLIGHT_YELLOW_PIN);
SimpleIO green_light(STOPLIGHT_GREEN_PIN);
SimpleIO valve1(VALVE1_PIN);
SimpleIO valve2(VALUE2_PIN);
Transducer transducer1(TRANSDUCER1_PIN);
Transducer transducer2(TRANSDUCER2_PIN);
Transducer transducer3(TRANSDUCER3_PIN);
// Loadcell loadcell(LOADCELL_PIN);
SimpleIO Switch_Oxidizer(OXYGEN_VALUE_PIN);
SimpleIO Switch_Fuel(FUEL_VALUE_PIN);
SimpleIO Switch_Ignition(IGNITION_PIN);
SimpleIO Switch_Launch(LAUNCH_PIN);

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


// TODO: make sure these are proper data typ es
struct SerialDate {
	u_int64_t time;
	u_int32_t tranducer1_data;
	u_int32_t tranducer2_data;
	int32_t tranducer3_data;
	int32_t loadcell_data;
};
struct SerialDate serial_data = {0, 0, 0, 0};
/**
 * This function prints the serial data in the following format:
 * tranducer1_data, tranducer2_data, tranducer3_data, loadcell_data
*/
void print_serial_data() {
	String data = String(serial_data.time) + "," + String(serial_data.tranducer1_data) + "," + 
					String(serial_data.tranducer2_data) + "," + String(serial_data.tranducer3_data) + 
					"," + String(serial_data.loadcell_data);
	Serial.println(data);
}


u_int64_t START_TIME_US;
void setup() {
	Serial.begin(115200);
	STATE = POWER_ON;  // <- uncomment this line to start in POWER_ON state
	Serial.println("POWER_ON");
	
	// while (1) {
	// 	if (digitalRead(START_PIN) == HIGH) break;
	// }
	Serial.println("Time(us),Ducer1(psi), Ducer2(psi), Ducer3(psi), Loadcell(lbs)");
	Serial.println("START");
	START_TIME_US = micros();
	print_serial_data();
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
	// TODO: data monitoring
	// serial_data.tranducer1_data = transducer1.get_value_psi();
	// serial_data.tranducer2_data = transducer2.get_value_psi();
	// serial_data.tranducer3_data = transducer3.get_value_psi();
	// serial_data.loadcell_data = loadcell.get_value_lbs();
	serial_data.time = micros() - START_TIME_US;
	serial_data.tranducer1_data = 10;
	serial_data.tranducer2_data = 100;
	serial_data.tranducer3_data = 1000;
	serial_data.loadcell_data = 10000;
	print_serial_data();
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

	//Checks if the switch is on and turns on the valves if true
	if (){

	} else {

	}
};

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
			Serial.println("IN TEST");
			test_state();
			STATE = FAIL;
			break;

		default:
			STATE = FAIL;  // something went wrong
	}
}
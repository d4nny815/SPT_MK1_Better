#include <Arduino.h>
#include <stdlib.h>
#include <avr/io.h>
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "SolenoidValve.h"
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
#define KSI_PIN 33
#define LAUNCH_BTN_PIN 34
#define SW_IGNITION 35

// OUTPUT PINS
#define STOPLIGHT_GREEN_PIN 4
#define STOPLIGHT_YELLOW_PIN 5
#define STOPLIGHT_RED_PIN 6
#define FUEL_VALUE_PIN 7
#define OXYGEN_VALVE_PIN 8
#define IGN_PIN 9
// PINS 10-13 are reserved for SPI communication

// Define the states of the system
enum State {
  POWER_ON,
  KSI,
  LAUNCH,
  FAIL
};
State STATE;
bool first_time_in_state = true;


// Instantiate every component in the system
SerialData serial_data;
DigitalOutput red_light(STOPLIGHT_RED_PIN);
DigitalOutput yellow_light(STOPLIGHT_YELLOW_PIN);
DigitalOutput green_light(STOPLIGHT_GREEN_PIN);
DigitalOutput ign_wire(IGN_PIN);
SoleinoidValve oxygen_valve(OXYGEN_VALVE_PIN, 100, 60);
SoleinoidValve fuel_valve(FUEL_VALUE_PIN, 100, 60);
DigitalInput ksi(KSI_PIN);
DigitalInput launch_btn(LAUNCH_BTN_PIN);
DigitalInput sw_fuel(SW_FUEL_VALUE_PIN);
DigitalInput sw_oxygen(SW_OXYGEN_VALUE_PIN);
DigitalInput sw_launch(SW_LAUNCH_PIN);
DigitalInput sw_ignition(SW_IGNITION);
DigitalInput estop(ESTOP_PIN);
Transducer ducer1(TRANSDUCER1_PIN);
Transducer ducer2(TRANSDUCER2_PIN);
Transducer ducer3(TRANSDUCER3_PIN);
Transducer ducer_arr[] = {ducer1, ducer2, ducer3};

void setup() {
	Serial.begin(115200);
	Serial.println("POWER_ON");
	serial_data.print_header();
	serial_data.set_start_time();
	STATE = POWER_ON; 
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
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
		green_light.turn_off();
		yellow_light.turn_on();
		red_light.turn_on();
		first_time_in_state = false;
	}

	serial_data.accummulate_data(ducer_arr);
	// serial_data.print_serial_data(); // TODO: uncomment this line to print data
	return;
};


void ksi_state () {
	if (first_time_in_state) {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
		green_light.turn_on();
		yellow_light.turn_off();
		red_light.turn_on();
		first_time_in_state = false;
	}

	if (sw_fuel.read() && sw_oxygen.read()) {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
	}
	else if (sw_fuel.read() && !sw_oxygen.read()) {
		fuel_valve.turn_on(millis());
		oxygen_valve.turn_off();
	}
	else if (sw_oxygen.read() && !sw_fuel.read()) {
		oxygen_valve.turn_on(millis());
		fuel_valve.turn_off();
	} 
	else {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
	}

	serial_data.accummulate_data(ducer_arr);
	// serial_data.print_serial_data();  // TODO: uncomment this line to print data
	return;
};

void launch_state () {
	if (first_time_in_state) {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
		green_light.turn_on();
		yellow_light.turn_on();
		red_light.turn_off();
		first_time_in_state = false;
	}
	
	if (sw_launch.read()) {
		oxygen_valve.turn_on(millis());
		fuel_valve.turn_on(millis());
	}
	else {
		oxygen_valve.turn_off();
		fuel_valve.turn_off();
	}

	if (sw_ignition.read()) ign_wire.turn_on();	
	else ign_wire.turn_off();


	serial_data.accummulate_data(ducer_arr);
	// serial_data.print_serial_data();
	return;
};

void fail_state () {
	green_light.toggle();
	yellow_light.toggle();
	red_light.toggle();

	delay(1000);

	ign_wire.turn_off();
	oxygen_valve.turn_off();
	fuel_valve.turn_off();

	serial_data.accummulate_data(ducer_arr);
	// serial_data.print_serial_data();
	return;
};


void loop() {
	if (!estop.read()) {
		STATE = FAIL;
	}

	// TODO: add serial communication
	// if (Serial.available()) {
	// }

	switch (STATE) {
		case (POWER_ON): 
			power_on_state();
			// Serial.println("POWERON state");
			if (ksi.read()) {
				STATE = KSI;
				first_time_in_state = true;
			}
			break;
		case (KSI): 
			ksi_state();
			// Serial.println("KSI state");

			if (launch_btn.read()) {
				STATE = LAUNCH;
				first_time_in_state = true;
			}
			if (!ksi.read()) {
				STATE = POWER_ON;
				first_time_in_state = true;
			}
			break;
		case (LAUNCH): 
			launch_state();
			// Serial.println("LAUNCH state");

			if (!ksi.read()) {
				STATE = POWER_ON;
				first_time_in_state = true;
			}
			break;
		case (FAIL): 
			fail_state();

			if (!ksi.read()) {
				STATE = POWER_ON;
				first_time_in_state = true;
			}
			break;

		default:
			STATE = POWER_ON;
	}
}
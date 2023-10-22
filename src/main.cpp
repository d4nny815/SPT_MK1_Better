#include <Arduino.h>

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

// OUTPUT PINS
// TODO: change for proper pin numbers and names
#define VALVE1_PIN 20
#define VALUE2_PIN 21
#define LOADCELL_PIN 22
#define DISPLAY_PIN 23
#define STOPLIGHT_PIN 24


enum State {
  POWER_ON,
  KEY_IN,
  KEY_TURNED,
  FAIL
};
State STATE;

void setup() {
	STATE = POWER_ON;
}

void power_on_state () {};
void key_in_state () {};
void key_turned_state () {};
void fail_state () {};

void loop() {
	switch (STATE) {
		case (POWER_ON): 
			// TODO: POWER_ON state
			break;

		case (KEY_IN): 
			// TODO: KEY_IN state
			break;

		case (KEY_TURNED): 
			// TODO: KEY_TURNED state
			break;

		case (FAIL): 
			// TODO: FAIL state
			break;

		default:
			STATE = FAIL;  // something went wrong
	}
}
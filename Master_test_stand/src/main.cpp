#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <driver/adc.h>
#include <accumulateData.h>
#include <../pins.h>
#include "DigitalOutput.h"
#include "SolenoidValve.h"
#include <Filters.h>
#include <Filters/Butterworth.hpp>

// // OUTPUT PINS
DigitalOutput red_light(STOPLIGHT_RED_PIN);  // lights are active low
DigitalOutput yellow_light(STOPLIGHT_YELLOW_PIN);  // lights are active low
DigitalOutput green_light(STOPLIGHT_GREEN_PIN);  // lights are active low
DigitalOutput ign_wire(IGN_PIN);
DigitalOutput out1(out1_pin);
DigitalOutput out2(out2_pin);
SoleinoidValve oxygen_valve(OXYGEN_VALVE_PIN, 100, 90);
SoleinoidValve fuel_valve(FUEL_VALUE_PIN, 100, 90);

/* State Machine */
enum SystemState {
	STATE_POWERON,
	STATE_KSI,
	STATE_LAUNCH,
	STATE_FAIL,
	STATE_TEST
};
SystemState STATE;
bool first_time_in_state = true;

// /* Control Signals */
uint8_t comms = 0x00;
const u_int8_t bit_ksi = 1 << 0;
const u_int8_t bit_launch_btn = 1 << 1;
const u_int8_t bit_sw_fuel = 1 << 2;
const u_int8_t bit_sw_oxygen = 1 << 3;
const u_int8_t bit_sw_launch = 1 << 4;
const u_int8_t bit_sw_ign = 1 << 5;
const u_int8_t bit_valid = 1 << 6;
u_int8_t bit_heartbeat = 1 << 7;

/* ESP NOW */
typedef struct {
	uint8_t button_data;
} incomingPacket_t;
incomingPacket_t incomingPacket;

esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0x08, 0xd1, 0xf9, 0xef, 0x32, 0x84};  // MAC Address of the Control Box
esp_err_t result;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  	// Serial.print("\r\nLast Packet Send Status:\t");
  	// Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  	memcpy(&incomingPacket, incomingData, sizeof(incomingPacket));
  	comms = incomingPacket.button_data;
	// Serial.println(comms, BIN);
}

/* Watchdog Timer */



// TODO: setup heartbeat timer

void setup() {
	Serial.begin(115200);

	// TODO: Setup Watchdog Timer

	// Setup ADC
	setup_ADC();
	
	// setup ESP_NOW
	WiFi.mode(WIFI_STA);
	esp_now_init();
	esp_now_register_send_cb(OnDataSent);
	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  	peerInfo.channel = 0;  
  	peerInfo.encrypt = false;
	// TODO: wait for esp_now_add_peer to return ESP_OK
	esp_now_add_peer(&peerInfo);
	esp_now_register_recv_cb(OnDataRecv);

	STATE = STATE_POWERON;
	Serial.println("||||||||Setup complete||||||||");
	set_start_time();
	xTaskCreate(accumulate_data, "accumulate_data", 2048, NULL, 1, NULL);
	// vTaskDelete(NULL);
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
		ign_wire.turn_off();
		oxygen_valve.close();
		fuel_valve.close();
		green_light.turn_off();
		yellow_light.turn_on();
		red_light.turn_on();
		first_time_in_state = false;
	}
	return;
}

void ksi_state () {
	if (first_time_in_state) {
		ign_wire.turn_off();
		oxygen_valve.close();
		fuel_valve.close();
		green_light.turn_on();
		yellow_light.turn_off();
		red_light.turn_on();
		// set_start_time();
		first_time_in_state = false;
	}

	if ((comms & bit_sw_fuel) && (comms & bit_sw_oxygen)) {
		oxygen_valve.close();
		fuel_valve.close();
	}
	else if ((comms & bit_sw_fuel) && !(comms & bit_sw_oxygen)) {
		fuel_valve.open();
		oxygen_valve.close();
	}
	else if ((comms & bit_sw_oxygen) && !(comms & bit_sw_fuel)) {
		oxygen_valve.open();
		fuel_valve.close();
	} 
	else {
		oxygen_valve.close();
		fuel_valve.close();
	}
	return;
}

void launch_state () {
	if (first_time_in_state) {
		ign_wire.turn_off();
		oxygen_valve.close();
		fuel_valve.close();
		green_light.turn_on();
		yellow_light.turn_on();
		red_light.turn_off();
		first_time_in_state = false;
	}
	
	if ((comms & bit_sw_launch)) {
		// TODO: add delay here oxygen -> 100ms -> fuel
		oxygen_valve.open(); 
		fuel_valve.open();
	}
	else {
		oxygen_valve.close();
		fuel_valve.close();
	}

	if ((comms & bit_sw_ign)) ign_wire.turn_on();	
	else ign_wire.turn_off();

	return;
}

void fail_state () {
	green_light.turn_off();
	yellow_light.turn_off();
	red_light.turn_off();

	ign_wire.turn_off();
	oxygen_valve.close();
	fuel_valve.close();
	return;
}

void test_state() {
	return;
}

void loop() {

	// if (!(digitalRead(ESTOP_PIN))) {
	// 	STATE = STATE_FAIL;
	// }

	switch (STATE) {
		case STATE_POWERON:
			power_on_state();

			if (comms & bit_ksi) {
				STATE = STATE_KSI;
				first_time_in_state = true;
			}
			break;
		case STATE_KSI:
			ksi_state();

			if (comms & bit_launch_btn) {
				STATE = STATE_LAUNCH;
				first_time_in_state = true;
			}
			if (!(comms & bit_ksi)) {
				STATE = STATE_POWERON;
				first_time_in_state = true;
			}
			break;
		case STATE_LAUNCH:
			launch_state();

			if (!(comms & bit_ksi)) {
				STATE = STATE_POWERON;
				first_time_in_state = true;
			}
			break;
		case STATE_FAIL:
			fail_state();

			if (comms == 0xC0 && digitalRead(ESTOP_PIN)) {
				STATE = STATE_POWERON;
				first_time_in_state = true;
			}
			break;
		case STATE_TEST:
			test_state();
			break;
		default:
			STATE = STATE_POWERON;
			break;
	}
	vTaskDelay(30 / portTICK_PERIOD_MS);
}

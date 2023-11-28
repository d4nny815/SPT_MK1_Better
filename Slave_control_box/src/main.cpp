/**
 * Project: MK1 Control Box
 * Author: Danny Gutierrez and Nick Katz
 * Description: reads the state of the switches and buttons and sends them to the MK1. Prints the incoming data from the MK1 to the serial monitor to be graphed.
*/

#include <Arduino.h>
#include <DigitalInput.h>
#include <esp_now.h>
#include <WiFi.h>

// INPUT PINS
//Digital IN
#define SW_IGN 23
#define SW_LAUNCH 22
#define SW_OXYGEN 21
#define SW_FUEL 19
#define BTN_LAUNCH 18
#define SW_KSI 5

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
unsigned long transmit_time = 500;

esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0x58, 0xcf, 0x79, 0xb3, 0xd8, 0x9c}; // MAC Address of the MK1
esp_err_t result;

typedef struct {
    uint32_t time_data;
    uint16_t transducer1_data;
    uint16_t transducer2_data;
    uint16_t transducer3_data;
    uint16_t transducer4_data;
    uint16_t transducer5_data;
    uint16_t transducer6_data;
    uint16_t transducer7_data;
    uint16_t transducer8_data;
    uint16_t thermoresistor1_data;
    uint16_t thermoresistor2_data;
    uint16_t thermoresistor3_data;
    uint16_t thermoresistor4_data;
    uint16_t thermoresistor5_data;
    uint16_t thermoresistor6_data;
    uint16_t loadcell_data;
} incomingPacket_t;

typedef struct {
	uint8_t button_data;
} outgoingPacket_t;

const int buffer_size = 6;
int buffer_index;
incomingPacket_t incomingPacket[buffer_size];
outgoingPacket_t outgoingPacket;
String success;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
      memcpy(&incomingPacket, incomingData, sizeof(incomingPacket));
    //   Serial.print("Bytes received: ");
    //   Serial.println(len);
    
    for (buffer_index = 0; buffer_index < buffer_size; buffer_index++) { 
        Serial.printf("%lu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu\n",
                        incomingPacket[buffer_index].time_data, incomingPacket[buffer_index].transducer1_data,
                        incomingPacket[buffer_index].transducer2_data, incomingPacket[buffer_index].transducer3_data,
                        incomingPacket[buffer_index].transducer4_data, incomingPacket[buffer_index].transducer5_data,
                        incomingPacket[buffer_index].transducer6_data, incomingPacket[buffer_index].transducer7_data,
                        incomingPacket[buffer_index].transducer8_data, incomingPacket[buffer_index].thermoresistor1_data,
                        incomingPacket[buffer_index].thermoresistor2_data, incomingPacket[buffer_index].thermoresistor3_data,
                        incomingPacket[buffer_index].thermoresistor4_data, incomingPacket[buffer_index].thermoresistor5_data,
                        incomingPacket[buffer_index].thermoresistor6_data, incomingPacket[buffer_index].loadcell_data 
        );
    }
    return;
}

void setup() {
  	Serial.begin(230400);
	transmit_data |= bit_valid;
  	transmit_data |= bit_heartbeat;

	// TODO: WATCHDOG SETUP
	

	// ESP-NOW SETUP
	WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_send_cb(OnDataSent);
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
    esp_now_register_recv_cb(OnDataRecv);

	Serial.println("POWER_ON");
}

//////////////////////////////////////////////////////////////////////////////////
//	Real Code Starts Here
//
//
//////////////////////////////////////////////////////////////////////////////////

void loop() {
  	// // read the KSI bit
	// if (ksi_sw.read()) transmit_data = transmit_data | bit_ksi;
	// else transmit_data = transmit_data & (bit_ksi ^ 0xff);

	// // read the IGNITION bit
	// if (launch_btn.read()) transmit_data = transmit_data | bit_launch_btn;
	// else transmit_data = transmit_data & (bit_launch_btn ^ 0xff);

	// // read the FUEL bit
	// if (fuel_sw.read()) transmit_data = transmit_data | bit_sw_fuel;
	// else transmit_data = transmit_data & (bit_sw_fuel ^ 0xff);

	// // read the OXYGEN bit
	// if (oxygen_sw.read()) transmit_data = transmit_data | bit_sw_oxygen;
	// else transmit_data = transmit_data & (bit_sw_oxygen ^ 0xff);

	// //read the LAUNCH bit
	// if (launch_sw.read()) transmit_data = transmit_data | bit_sw_launch;
	// else transmit_data = transmit_data & (bit_sw_launch ^ 0xff);

	// //read the IGNITION bit
	// if (ign_sw.read()) transmit_data = transmit_data | bit_sw_ign;
	// else transmit_data = transmit_data & (bit_sw_ign ^ 0xff);

  	if (Serial.available()) {
        uint8_t incomingByte = Serial.read();
        outgoingPacket.button_data = incomingByte;
    }

    cur_time = millis();
    if (cur_time - prev_time_sent >= transmit_time) {
        prev_time_sent = cur_time;
        result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingPacket, sizeof(outgoingPacket));
        if (result == ESP_OK) {
            Serial.println("Sent with success");
        }
        else {
            Serial.println("Error sending the data");
        }
    }
}

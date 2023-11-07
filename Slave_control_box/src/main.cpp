#include <Arduino.h>
#include <DigitalInput.h>

// INPUT PINS
//Digital IN
#define SW_IGN 14
#define SW_LAUNCH 15
#define SW_OXYGEN 16
#define SW_FUEL 17
#define BTN_LAUNCH 18
#define SW_KSI 19

#define BAUD_RATE 9600 //

// Instantiate every component in the system
DigitalInput  ign_sw(SW_IGN);
DigitalInput  launch_sw(SW_LAUNCH);
DigitalInput  oxygen_sw(SW_OXYGEN);
DigitalInput  fuel_sw(SW_FUEL);
DigitalInput  launch_btn(BTN_LAUNCH);
DigitalInput  ksi_sw(SW_KSI);

// b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0
// heartbeat | valid | sw_ign | sw_launch | sw_oxygen | sw_fuel | launch_btn | ksi 
int comms_buffer;
uint8_t comms = 0x00;
const u_int8_t bit_ksi = 1;
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
	Serial.println("POWER_ON");
  comms |= bit_valid;
  comms |= bit_heartbeat;
}

void loop() {

//changes the KSI bit
  if (ksi_sw.read()){
    comms = comms|bit_ksi;
  }
  else {
    comms = comms & (bit_ksi ^ 0xff);
  }

 //changes the IGNITION bit
   if (launch_btn.read()){
     comms = comms|bit_launch_btn;
   }
   else {
     comms = comms & (bit_launch_btn ^ 0xff);
   }

 //changes the FUEL bit
   if (fuel_sw.read()){
     comms = comms|bit_sw_fuel;
   }
   else {
     comms = comms & (bit_sw_fuel ^ 0xff);
   }

 //changes the OXYGEN bit
   if (oxygen_sw.read()){
     comms = comms|bit_sw_oxygen;
   }
   else {
     comms = comms & (bit_sw_oxygen ^ 0xff);
   }

 //changes the LAUNCH bit
   if (launch_sw.read()){
     comms = comms|bit_sw_launch;
   }
   else {
     comms = comms & (bit_sw_launch ^ 0xff);
   }

 //changes the IGNITION bit
   if (ign_sw.read()){
     comms = comms|bit_sw_ign;
   }
   else {
     comms = comms & (bit_sw_ign ^ 0xff);
   }

Serial1.write(comms);
 Serial.print("bits: ");
Serial.println(comms, BIN);

delay(1000);
}

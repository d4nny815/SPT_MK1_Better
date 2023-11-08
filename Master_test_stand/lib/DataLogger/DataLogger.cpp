#include "DataLogger.h"
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"


/**
 * @brief Constructor for the SerialData class
*/
SerialData::SerialData() {
	time = 0;
	transducer1_data = 0;
	transducer2_data = 0;
	transducer3_data = 0;
	loadcell_data = 0;
}

void SerialData::set_start_time() {
	START_TIME_US = micros();
	return;
}


//TODO: add loadcell data
/**
 * @brief Accumulates the data from the transducers
 * @param transducers_arr array of transducers
*/
void SerialData::accummulate_data(Transducer transducers_arr[]) {
	time = micros() - START_TIME_US;
	// transducer1_data = transducers_arr[0].get_PSI();
	// transducer2_data = transducers_arr[1].get_PSI();
	// transducer3_data = transducers_arr[2].get_PSI();
	transducer1_data = 0xdead;
	transducer2_data = 0xbeef;
	transducer3_data = 0xabcd;
	loadcell_data = 0x91ef;
	return;
}


/**
 * @brief Prints the data to the serial monitor
*/
void SerialData::print_header() {
	Serial.println("Time(us),Ducer1(psi), Ducer2(psi), Ducer3(psi), Loadcell(lbs)");
	return;
}



/**
 * @brief Prints the data to the serial monitor
*/
void SerialData::print_serial_data() {
    String data = String(time) + "," + String(transducer1_data) + "," + 
					String(transducer2_data) + "," + String(transducer3_data) + 
					"," + String(loadcell_data);
	Serial.println(data);
    return;
}

void SerialData::send_serial_data() {
		buffer[0] = (time >> 56) & 0xff; 	
		buffer[1] = (time >> 48) & 0xff; 	
		buffer[2] = (time >> 40) & 0xff; 	
		buffer[3] = (time >> 32) & 0xff; 	
		buffer[4] = (time >> 24) & 0xff; 	
		buffer[5] = (time >> 16) & 0xff; 	
		buffer[6] = (time >> 8) & 0xff; 	
		buffer[7] = (time) & 0xff; 

		buffer[8] = (transducer1_data >> 8) & 0xff;
		buffer[9] = (transducer1_data) & 0xff;	

		buffer[10] = (transducer2_data >> 8) & 0xff;	
		buffer[11] = (transducer2_data) & 0xff;	

		buffer[12] = (transducer3_data >> 8) & 0xff;	
		buffer[13] = (transducer3_data) & 0xff;	

		buffer[14] = (loadcell_data >> 8) & 0xff;	
		buffer[15] = (loadcell_data) & 0xff;

		// Serial.print("buffer: ");
		// Serial.write(buffer, 16);

		// String data;
		// for (int i=0; i<16; i++) {
		// 	data += String(buffer[i], HEX);
		// }
		// Serial.print(" -> buffer string: ");
		// Serial.println(data);
		Serial1.write(buffer, 16);	
		delay(50);

	return;
}
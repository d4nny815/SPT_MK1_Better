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
	START_TIME_US = millis();
	return;
}


/**
 * @brief Accumulates the data from the transducers
 * @param transducers_arr array of transducers
*/
void SerialData::accummulate_data(Transducer transducers_arr[], Loadcell loadcell) {
	time = millis() - START_TIME_US;
	transducer1_data = transducers_arr[0].get_PSI();
	transducer2_data = transducers_arr[1].get_PSI();
	transducer3_data = transducers_arr[2].get_PSI();
	loadcell_data = loadcell.get_lbs();
	// transducer1_data = 0xdead;
	// transducer2_data = 0xbeef;
	// transducer3_data = 0xabcd;
	// loadcell_data = 0x91ef;
	return;
}


/**
 * @brief Prints the data to the serial monitor
*/
void SerialData::print_header() {
	// Serial.println("Time(us),Ducer1(psi), Ducer2(psi), Ducer3(psi), Loadcell(lbs)");
	return;
}


/**
 * @brief Prints the data to the serial monitor
*/
void SerialData::print_serial_data() {
    data = String(time) + "," + String(transducer1_data) + "," + 
					String(transducer2_data) + "," + String(transducer3_data) + 
					"," + String(loadcell_data);

	_cur_time = millis();
	if (_cur_time - _prev_time_check >= _transmit_time) {
		_prev_time_check = _cur_time;
		Serial.println(data);
	}
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

		// uint64_t rx_time = buffer[0] << 56 | buffer[1] << 48 | buffer[2] << 40 
		// 					| buffer[3] << 32 | buffer[4] << 24 | buffer[5] << 16 
		// 					| buffer[6] << 8 | buffer[7];
		// uint16_t rx_ducer1 = buffer[8] << 8 | buffer[9];
		// uint16_t rx_ducer2 = buffer[10] << 8 | buffer[11];
		// uint16_t rx_ducer3 = buffer[12] << 8 | buffer[13];
		// uint16_t rx_loadcell = buffer[14] << 8 | buffer[15];

		// String rx_data = "RX: " + String(rx_time) + "," + String(rx_ducer1) + "," + 
		// 			String(rx_ducer2) + "," + String(rx_ducer3) + 
		// 			"," + String(rx_loadcell);

		_cur_time = millis();
		if (_cur_time - _prev_time_check >= _transmit_time) {
			_prev_time_check = _cur_time;
			// rx_time = rx_ducer1 = rx_ducer2 = rx_ducer3 = rx_loadcell = 0;
			// Serial.println(rx_data);
			Serial1.write(buffer, 16);	
		}
	return;
}
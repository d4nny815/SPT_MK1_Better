#include "DataLogger.h"
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"


/**
 * @brief Constructor for the SerialData class
*/
SerialData::SerialData() {
	START_TIME_US = micros();
	time = 0;
	transducer1_data = 0;
	transducer2_data = 0;
	transducer3_data = 0;
	loadcell_data = 0;
}

//TODO: add loadcell data
/**
 * @brief Accumulates the data from the transducers
 * @param transducers_arr array of transducers
*/
void SerialData::accummulate_data(Transducer transducers_arr[]) {
	time = micros();
	transducer1_data = transducers_arr[0].get_PSI();
	transducer2_data = transducers_arr[1].get_PSI();
	transducer3_data = transducers_arr[2].get_PSI();
	loadcell_data = 0;
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
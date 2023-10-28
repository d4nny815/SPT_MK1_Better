#include "DataLogger.h"
#include <Arduino.h>


/**
 * @brief Prints the data to the serial monitor
*/
void print_header() {
	Serial.println("Time(us),Ducer1(psi), Ducer2(psi), Ducer3(psi), Loadcell(lbs)");
	return;
}

void print_serial_data(struct SerialData* serial_data) {
    String data = String(serial_data->time) + "," + String(serial_data->tranducer1_data) + "," + 
					String(serial_data->tranducer2_data) + "," + String(serial_data->tranducer3_data) + 
					"," + String(serial_data->loadcell_data);
	Serial.println(data);
    return;
}
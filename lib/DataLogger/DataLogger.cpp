#include "DataLogger.h"
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"

// void accummulate_data(struct SerialData* serial_data, Transducer transducers_arr[], Loadcell loadcell) {
	// serial_data->time = micros() - START_TIME_US;
	// serial_data->tranducer1_data = transducers_arr[0].voltageToPSI(1);
	// serial_data->tranducer2_data = transducers_arr[1].voltageToPSI(1);
	// serial_data->tranducer3_data = transducers_arr[2].voltageToPSI(1);
	// serial_data.loadcell_data = loadcell.read();
// }



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
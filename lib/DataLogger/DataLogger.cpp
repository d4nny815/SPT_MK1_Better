#include "DataLogger.h"
#include <Arduino.h>


/**
 * @brief Prints the data to the serial monitor
*/
void print_serial_data(struct SerialData* serial_data) {
    String data = String(serial_data->time) + "," + String(serial_data->tranducer1_data) + "," + 
					String(serial_data->tranducer2_data) + "," + String(serial_data->tranducer3_data) + 
					"," + String(serial_data->loadcell_data);
	Serial.println(data);
    return;
}
#include "DataLogger.h"
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"


/**
 * @brief Constructor for the SerialData class
*/
SerialData::SerialData() {
	random_var = 0;
}

void SerialData::set_start_time() {
	START_TIME_MS = millis();
	return;
}

/**
 * @brief Accumulates the data from the transducers
 * @param transducers_arr array of transducers
 * @param thermoresistor_arr array of thermoresistors
 * @param loadcell loadcell object
*/
void SerialData::accummulate_data(Transducer transducers_arr[], Thermoresistor thermoresistor_arr[], Loadcell loadcell) {
	// outgoingPacket.time_data = millis() - START_TIME_MS;
	// outgoingPacket.transducer1_data = transducers_arr[0].get_PSI();
	// outgoingPacket.transducer2_data = transducers_arr[1].get_PSI();
	// outgoingPacket.transducer3_data = transducers_arr[2].get_PSI();
	// outgoingPacket.transducer4_data = transducers_arr[3].get_PSI();
	// outgoingPacket.transducer5_data = transducers_arr[4].get_PSI();
	// outgoingPacket.transducer6_data = transducers_arr[5].get_PSI();
	// outgoingPacket.thermoresistor1_data = thermoresistor_arr[0].get_temp();
	// outgoingPacket.thermoresistor2_data = thermoresistor_arr[1].get_temp();
	// outgoingPacket.thermoresistor3_data = thermoresistor_arr[2].get_temp();
	// outgoingPacket.loadcell_data = loadcell.get_lbs();
	outgoingDataPacket[buffer_index].time_data = millis() - START_TIME_MS;
	outgoingDataPacket[buffer_index].transducer1_data = random_var;
	outgoingDataPacket[buffer_index].transducer2_data = random_var * 2;
	outgoingDataPacket[buffer_index].transducer3_data = random_var * 3;
	outgoingDataPacket[buffer_index].transducer4_data = random_var * 4;
	outgoingDataPacket[buffer_index].transducer5_data = random_var % 10000;
	outgoingDataPacket[buffer_index].transducer6_data = random_var % 1000;
	outgoingDataPacket[buffer_index].thermoresistor1_data = pow(random_var, 2);
	outgoingDataPacket[buffer_index].thermoresistor2_data = random_var - 10000;
	outgoingDataPacket[buffer_index].thermoresistor3_data = random_var ^ 0xFFFF;
	outgoingDataPacket[buffer_index].loadcell_data = 0x4567;
	random_var++; // for testing
	return;
}


/**
 * @brief Prints the data to the serial monitor
*/
void SerialData::print_serial_data() {\
    Serial.printf("%d,%X,%X,%X,%X,%X,%X,%X,%X,%X,%X\n",
					outgoingDataPacket[buffer_index].time_data, outgoingDataPacket[buffer_index].transducer1_data, outgoingDataPacket[buffer_index].transducer2_data, 
					outgoingDataPacket[buffer_index].transducer3_data, outgoingDataPacket[buffer_index].transducer4_data, outgoingDataPacket[buffer_index].transducer5_data, 
					outgoingDataPacket[buffer_index].transducer6_data, outgoingDataPacket[buffer_index].thermoresistor1_data, outgoingDataPacket[buffer_index].thermoresistor2_data, 
					outgoingDataPacket[buffer_index].thermoresistor3_data, outgoingDataPacket[buffer_index].loadcell_data, buffer_index);
    return;
}
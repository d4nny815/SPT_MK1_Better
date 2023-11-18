#ifndef DataLogger_h
#define DataLogger_h
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"
#include "Thermoresistor.h"
#include <WiFi.h>
#include <esp_now.h>


class SerialData {
	private:
		uint32_t START_TIME_MS;
		
		uint32_t random_var;
	public:
		typedef struct {
			uint8_t button_data;
			uint32_t time_data;
			uint16_t transducer1_data;
			uint16_t transducer2_data;
			uint16_t transducer3_data;
			uint16_t transducer4_data;
			uint16_t transducer5_data;
			uint16_t transducer6_data;
			uint16_t thermoresistor1_data;
			uint16_t thermoresistor2_data;
			uint16_t thermoresistor3_data;
			uint16_t loadcell_data;
		} dataPacket;
		dataPacket outgoingDataPacket[8];
		uint8_t buffer_index;

		SerialData();
		void set_start_time();
		void accummulate_data(Transducer transducers_arr[], Thermoresistor thermoresistor_arr[], Loadcell loadcell);
		void print_serial_data();
};
#endif

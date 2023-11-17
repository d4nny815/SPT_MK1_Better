#ifndef DataLogger_h
#define DataLogger_h
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"


class SerialData {
	private:
	uint64_t time;
	uint64_t START_TIME_US;
	uint16_t transducer1_data;
	uint16_t transducer2_data;
	uint16_t transducer3_data;
	uint16_t loadcell_data;

	String data;
	byte buffer[16];
	
	unsigned long _cur_time;
    unsigned long _prev_time_check;
    unsigned long _transmit_time = 30;

	public:
		SerialData();
		void accummulate_data(Transducer transducers_arr[], Loadcell Loadcell);
		void set_start_time();
		void print_header();
		void print_serial_data();
		void send_serial_data();
};
#endif

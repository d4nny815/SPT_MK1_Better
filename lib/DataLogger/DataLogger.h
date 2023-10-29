#ifndef DataLogger_h
#define DataLogger_h
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"


class SerialData {
	private:
	u_int64_t time;
	u_int64_t START_TIME_US;
	u_int16_t transducer1_data;
	u_int16_t transducer2_data;
	u_int16_t transducer3_data;
	int32_t loadcell_data;


	public:
		SerialData();
		// void accummulate_data(Transducer transducers_arr[], Loadcell Loadcell); // TODO: implement this
		void set_start_time();
		void accummulate_data(Transducer transducers_arr[]);
		void print_header();
		void print_serial_data();
};
#endif

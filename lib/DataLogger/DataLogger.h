#ifndef DataLogger_h
#define DataLogger_h
#include <Arduino.h>
#include "Transducer.h"
#include "Loadcell.h"

// TODO: make sure these are proper data types


struct SerialData {
	u_int64_t time;
	u_int32_t tranducer1_data;
	u_int32_t tranducer2_data;
	int32_t tranducer3_data;
	int32_t loadcell_data;
};

// void accummulate_data(struct SerialData* serial_data, Transducer transducers_arr[], Loadcell loadcell);
void print_header();
void print_serial_data(struct SerialData* serial_data);

#endif

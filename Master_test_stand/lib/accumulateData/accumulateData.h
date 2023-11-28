#ifndef accumulateData_h
#define accumulateData_h
#include <Arduino.h>
#include <../pins.h>
#include <Filters.h>
#include <Filters/Butterworth.hpp>

void setup_ADC();
void accumulate_data(void* parameter);
void set_start_time();

#endif
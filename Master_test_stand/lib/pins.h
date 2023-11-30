#ifndef pins_h
#define pins_h
#include <driver/adc.h>

// INPUT PINS
// ANALOG PINS
#define TRANSDUCER1_PIN ADC1_CHANNEL_3 // GPIO 4
#define TRANSDUCER2_PIN ADC1_CHANNEL_4 // GPIO 5
#define TRANSDUCER3_PIN ADC1_CHANNEL_5 // GPIO 6
#define TRANSDUCER4_PIN ADC1_CHANNEL_6 // GPIO 7
#define TRANSDUCER5_PIN ADC2_CHANNEL_4 // GPIO 15
#define TRANSDUCER6_PIN ADC2_CHANNEL_5 // GPIO 16
#define TRANSDUCER7_PIN ADC2_CHANNEL_6 // GPIO 17
#define TRANSDUCER8_PIN ADC2_CHANNEL_7 // GPIO 18

#define THERMISTOR1_PIN ADC1_CHANNEL_7 // GPIO 8
#define THERMISTOR2_PIN ADC1_CHANNEL_2 // GPIO 3
#define THERMISTOR3_PIN ADC1_CHANNEL_8 // GPIO 9
#define THERMISTOR4_PIN ADC1_CHANNEL_9 // GPIO 10
#define THERMISTOR5_PIN ADC2_CHANNEL_0 // GPIO 12
#define THERMISTOR6_PIN ADC2_CHANNEL_1 // GPIO 13

#define LOADCELL_PIN ADC2_CHANNEL_3 // GPIO 14
// DIGITAL PINS
#define ESTOP_SENSE 34

// OUTPUT PINS
#define STOPLIGHT_GREEN_PIN 33
#define STOPLIGHT_YELLOW_PIN 21
#define STOPLIGHT_RED_PIN 20
#define OXYGEN_VALVE_PIN 1
#define FUEL_VALUE_PIN 2
#define IGN_PIN 18
#define VALVE1_PIN 37
#define VALVE2_PIN 36
#define VALVE3_PIN 35
#define VALVE4_PIN 19
#define VALVE5_PIN 17
#define ESTOP_ENABLE_PIN 14


#endif
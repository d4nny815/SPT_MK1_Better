#ifndef SimpleIO_h
#define SimpleIO_h
#include <Arduino.h>

/**
 *   Simple IO libray
 *   Used for LEDS, relays, etc. Simple HIGH/LOW control signals 
 *
**/
class DigitalOutput {
    private:
        int _pin;
        bool _IO_state;

    public:
        DigitalOutput(int pin);
        bool get_state();
        void turn_on();
        void turn_on(int duty_cycle);
        void turn_off();
        void toggle();

};
#endif
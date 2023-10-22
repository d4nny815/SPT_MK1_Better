#ifndef SimpleIO_h
#define SimpleIO_h
#include <Arduino.h>

/**
 *   Simple IO libray
 *   Used for LEDS, relays, etc. Simple HIGH/LOW control signals 
 *
**/
class SimpleIO {
    private:
        int _pin;
        bool _IO_state;

    public:
        SimpleIO(int pin);
        bool get_state();
        void turn_on();
        void turn_off();
        void toggle();

};
#endif
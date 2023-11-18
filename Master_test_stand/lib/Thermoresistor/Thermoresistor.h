#ifndef Thermoresistor_h
#define Thermoresistor_h
#include <Arduino.h>

class Thermoresistor {
    private:
        int _pin;
        uint16_t _raw_reading;
        
    public:
        Thermoresistor(int pin);
        uint16_t get_temp();
};
#endif


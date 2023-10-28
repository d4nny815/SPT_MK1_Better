#ifndef Transducer_h
#define Transducer_h
#include <Arduino.h>
#include <stdlib.h>

class Transducer {
    private:
        int _pin;
        double _minVolt;
        double _maxVolt;
        
    public:
        Transducer(int pin);
        u_int16_t get_PSI();
};
#endif


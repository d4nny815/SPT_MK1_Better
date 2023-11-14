#ifndef Transducer_h
#define Transducer_h
#include <Arduino.h>

class Transducer {
    private:
        int _pin;
        uint16_t _raw_reading;
        uint16_t _minVolt;
        uint16_t _maxVolt;
        double _minCurrent = .004; // 4mA
        double _maxCurrent = .02; // 20mA
        uint16_t _resistor = 100; // 100 ohms
        
    public:
        Transducer(int pin);
        uint16_t get_PSI();
};
#endif


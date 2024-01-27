#ifndef Loadcell_h
#define Loadcell_h
#include <Arduino.h>


class Loadcell{
    private:
        int _pin;
        uint16_t _loadcell_rating_lbs;
        uint16_t _raw_reading;
        uint16_t _minVolt;
        uint16_t _maxVolt;
        double _minCurrent = .004; // 4mA
        double _maxCurrent = .02; // 20mA
        uint16_t _resistor = 100; // 100 ohms
        
    public:
        Loadcell(int pin, uint16_t loadcell_rating);
        uint16_t get_lbs(); 
};

#endif
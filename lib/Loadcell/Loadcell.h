#ifndef Loadcell_h
#define Loadcell_h

#include <Arduino.h>
#include "HX711.h"

/**
 *  Loadcell Library built from HX711 Library
 *   
 * 
 * 
 * 
 * 
 * 
 * **/


class Loadcell{
    private:
        HX711 _loadcell;
        int _dout_pin;
        int _clk_pin;
        float _calibration_factor;
    public:
        Loadcell(int dout_pin, int sck_pin, float calibration_factor);
        void setup();
        double get_units_lbs();   
};

#endif
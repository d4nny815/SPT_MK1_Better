#include <stdlib.h>
#include <Arduino.h>
#include "Transducer.h"


/**
 * @brief Constructor for the Transducer class
*/
Transducer::Transducer(int pin) {
    _pin = pin;
    pinMode(_pin, INPUT);
    _minVolt = ((.004 * 100) / 3.3) * 1024;
    _maxVolt = ((.02 * 100) / 3.3) * 1024; 
}

/**
 * @brief Returns the PSI of the transducer
 * @return PSI of the transducer from 0 to 1000
*/
u_int16_t Transducer::get_PSI(){
    int raw = analogRead(_pin);
    return map(raw, _minVolt, _maxVolt, 0, 1000);
}

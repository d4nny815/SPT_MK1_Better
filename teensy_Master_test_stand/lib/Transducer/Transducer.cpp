#include <stdlib.h>
#include <Arduino.h>
#include "Transducer.h"


/**
 * @brief Constructor for Transducer class
*/
Transducer::Transducer(int pin) {
    _pin = pin;
    pinMode(_pin, INPUT);
    _minVolt = (uint16_t)(_minCurrent * _resistor / 3.3 * 1024);
    _maxVolt = (uint16_t)(_maxCurrent * _resistor / 3.3 * 1024);

}

/**
 * @brief Reads the analog voltage from the transducer and converts it to PSI
 * @return PSI value
*/
uint16_t Transducer::get_PSI(){
    _raw_reading = analogRead(_pin);
    if (_raw_reading < _minVolt) {
        _raw_reading = _minVolt;
    }
    else if (_raw_reading > _maxVolt) {
        _raw_reading = _maxVolt;
    }
    return map(_raw_reading, _minVolt, _maxVolt, 0, 1000);
}

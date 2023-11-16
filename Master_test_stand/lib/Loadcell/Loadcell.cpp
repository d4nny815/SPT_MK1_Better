#include "Loadcell.h"
#include "Arduino.h"

Loadcell::Loadcell(int pin, uint16_t loadcell_rating) {
    _pin = pin;
    _loadcell_rating_lbs = loadcell_rating * 2.2;
    pinMode(_pin, INPUT);
    _minVolt = (uint16_t)(_minCurrent * _resistor / 3.3 * 1024);
    _maxVolt = (uint16_t)(_maxCurrent * _resistor / 3.3 * 1024);

}

uint16_t Loadcell::get_lbs() {
    _raw_reading = analogRead(_pin);
    // Serial.print(_raw_reading);
    // Serial.print(" -> ");
    if (_raw_reading < _minVolt) {
        _raw_reading = _minVolt;
    }
    else if (_raw_reading > _maxVolt) {
        _raw_reading = _maxVolt;
    }
    // Serial.println(map(_raw_reading, _minVolt, _maxVolt, 0, _loadcell_rating_lbs));
    return map(_raw_reading, _minVolt, _maxVolt, 0, _loadcell_rating_lbs);
}




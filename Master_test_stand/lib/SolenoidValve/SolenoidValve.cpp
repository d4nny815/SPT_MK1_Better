#include "SolenoidValve.h"
#include <Arduino.h>


/**
 * @brief Constructor for SoleinoidValve class
 * @param pin The pin number of the digital input
 * @param pullin_voltage The voltage percentage to turn on the solenoid valve
 * @param holding_voltage The voltage percentage to hold the solenoid valve
 * @return SoleinoidValve object
*/
SoleinoidValve::SoleinoidValve(int pin, uint8_t pullin_voltage, uint8_t holding_voltage) {
    _pin = pin;
    _IO_state = 0;
    _pull_in_voltage = (uint8_t)(constrain(pullin_voltage, 0, 100) / 100.0 * 255);
    _holding_voltage = (uint8_t)(constrain(holding_voltage, 0, 100) / 100.0 * 255);
    _in_pull_in_state = false;
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, 0);
}

bool SoleinoidValve::get_state() {return _IO_state; }


/**
 * @brief turn on the solenoid valve
*/
void SoleinoidValve::open() {
    if (_IO_state) return;
    if (!_in_pull_in_state) {
        analogWrite(_pin, _pull_in_voltage);
        _in_pull_in_state = true;
        _previous_time_check = millis();
        return;
    }
    _current_time = millis();
    if (_current_time - _previous_time_check < _pull_in_time_interval) return;
    _previous_time_check = _current_time;
    analogWrite(_pin, _holding_voltage);
    Serial.printf("opened on pin %d\n", _pin);
    _IO_state = 1;
    return;
}


/**
 * @brief turn off the solenoid valve
*/
void SoleinoidValve::close() {
    if (!_IO_state) return;
    _IO_state = 0;
    _in_pull_in_state = false;
    analogWrite(_pin, 0);
    return;
}
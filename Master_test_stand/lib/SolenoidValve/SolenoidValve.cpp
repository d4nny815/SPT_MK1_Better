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
    _pullin_voltage = constrain(pullin_voltage, 0, 100) / 100.0;
    _holding_voltage = constrain(holding_voltage, 0, 100) / 100.0;
    _previous_time_check = millis();
    _is_on = false;
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, 0);
}

bool SoleinoidValve::get_state() {return _IO_state; }


/**
 * @brief turn on the solenoid valve at pullin voltage for a certain amount of time then use holding voltage
*/
void SoleinoidValve::turn_on() {
    if (_IO_state) return;    
    if (!_is_on) {
        _current_time = millis();
        analogWrite(_pin, (int) (256 * _pullin_voltage));
        if (_current_time - _previous_time_check > _pull_in_time_interval) {
            Serial.print("turned on");
            Serial.println(_pin);
            _previous_time_check = _current_time;
            analogWrite(_pin, (int) (256 * _holding_voltage));
            _is_on = true;
            _IO_state = 1;
        }
    }  
    return;
}


/**
 * @brief Turn off (set to LOW) the output pin.
 */
void SoleinoidValve::turn_off() {
    if (!_IO_state) return;
    _IO_state = 0;
    _is_on = false;
    analogWrite(_pin, 0);
    return;
}
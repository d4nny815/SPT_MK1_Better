#include "SolenoidValve.h"
#include <Arduino.h>


/**
 * @brief Constructor for DigitalOutput class
 * @param pin_number - pin that component is plugged into on the board
 * @param pullin_voltage - percentage of the solenoid voltage that is required to pull in the solenoid
 * @param holding_voltage - percentage of the solenoid voltage that is required to hold the solenoid
 * @return DigitalOutput object
*/
SoleinoidValve::SoleinoidValve(int pin, u_int8_t pullin_voltage, u_int8_t holding_voltage) {
    _pin = pin;
    _IO_state = 0;
    _pullin_voltage = (pullin_voltage % 101) / 100.0;
    _holding_voltage = (holding_voltage % 101) / 100.0;
    _previous_time_check = 0;
    _is_on = false;
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, 0);
}

bool SoleinoidValve::get_state() {return _IO_state; }


/**
 * @brief turn on the solenoid valve at pullin voltage for a certain amount of time then use holding voltage
 * @param cur_time - current time in milliseconds
*/
void SoleinoidValve::turn_on(unsigned long cur_time) {
    if (_IO_state) return;    
    if (!_is_on) {
        analogWrite(_pin, (int) (256 * _pullin_voltage));
        if (cur_time - _previous_time_check > _pull_in_time_interval) {
            _previous_time_check = cur_time;
            _is_on = true;
            _IO_state = 1;
            Serial.print(_pin);
            Serial.println(" turned ON");
        }
    } else {
        analogWrite(_pin, (int) (256 * _holding_voltage));
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
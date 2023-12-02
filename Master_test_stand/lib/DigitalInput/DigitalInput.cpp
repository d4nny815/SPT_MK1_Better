#include "DigitalInput.h"
#include <Arduino.h>


/**
 * @brief Constructor for DigitalInput class
 * @param pin The pin number of the digital input
 * @return DigitalInput object
*/
DigitalInput::DigitalInput(int pin) {
    _pin = pin;
    pinMode(_pin, INPUT);
}

/**
 * @brief Reads the digital input
 * @return The state of the digital input
 * @note This function also debounces the input
*/
bool DigitalInput::read() {
    _btn_state = digitalRead(_pin);
    // _cur_time = micros();

    // if (_cur_time - _prev_time_check >= _debounce_time_us) {
    //     _prev_time_check = _cur_time;
    //     _last_btn_state = _btn_state;
    //     return _btn_state;
    // }

    return _last_btn_state;
}
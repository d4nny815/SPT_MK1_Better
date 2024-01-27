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
 * @brief Reads the state of the digital input and debounces it
 * @return The state of the digital input
*/
bool DigitalInput::read() {
    _btn_state = digitalRead(_pin);
    _cur_time = micros();

    // Check if the button state has changed
    if (_btn_state != _last_btn_state) {
        // Record the time of the state change for debounce
        _prev_time_check = micros();
    }

    // Check if enough time has passed since the last state change
    if (_cur_time - _prev_time_check >= _debounce_time_us) {
        // Return the current button state
        return _btn_state;
    }

    // Return the previous button state if debounce time hasn't passed
    return _last_btn_state;
}
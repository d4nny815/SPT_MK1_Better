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

// TODO: better implementation of debouncing
/**
 * @brief Reads the value of the digital input pin
*/
bool DigitalInput::read() {
    bool initial_read = digitalRead(_pin);
    delayMicroseconds(100); // terrible way to debounce
    return digitalRead(_pin) && (digitalRead(_pin) == initial_read);
}
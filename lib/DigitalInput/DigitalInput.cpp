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
 * @brief Reads the value of the digital input pin
*/
bool DigitalInput::read() {
    return digitalRead(_pin);
}
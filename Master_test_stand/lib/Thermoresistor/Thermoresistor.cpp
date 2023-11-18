#include "Thermoresistor.h"
#include <Arduino.h>

/**
 * @brief Constructor for the Thermoresistor class
 * @param pin the pin that the thermoresistor is connected to
*/
Thermoresistor::Thermoresistor(int pin) {
    _pin = pin;
    _raw_reading = 0;
}

/**
 * @brief Gets the temperature from the thermoresistor
 * @return the temperature in degrees celsius
*/
uint16_t Thermoresistor::get_temp() {
    _raw_reading = analogRead(_pin);
    return _raw_reading;
}
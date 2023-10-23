/**
 * Example for making your own Arduino module
 * Use syntax and structure from this example to make your own module
 * glhf :)
*/

#ifndef ARDUINO_CLASS_H
#define ARDUINO_CLASS_H

#include <Arduino.h>
#include <stdlib.h> // <- use std data types

class ArduinoClass {
    private:
        int16_t _examplePrivateVariable;
        int16_t _pin;

        int16_t _examplePrivateFunction(int16_t exampleParameter);
    public:
        ArduinoClass(); // Constructor
        void exampleFunction();
        u_int16_t get_value();
};

#endif 
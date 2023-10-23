#include "ArduinoClass.h"
#include "Arduino.h"
#include <stdlib.h>


// Constructor
ArduinoClass::ArduinoClass() {
    _examplePrivateVariable = 0;
    _pin = 0;
}

/**
 * This function is an example of a public function. just prints "Hello World!" to the serial monitor.
 * 
*/
void ArduinoClass::exampleFunction() {
    Serial.println("Hello World!");
}

/**
 * This function is an example of a public function. just prints "Hello World!" to the serial monitor.
 * 
*/
u_int16_t ArduinoClass::get_value() {
    return _examplePrivateVariable;
}

/**
 * This function is an example of a private function
 * @param exampleParameter: an example parameter you enter in
 * @return value after being modified by the function
 * 
*/
int16_t ArduinoClass::_examplePrivateFunction(int16_t exampleParameter) {
    return (exampleParameter * 2 - 1) * (exampleParameter * 2 - 1) ;
}



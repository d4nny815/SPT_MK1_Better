#include "SimpleIO.h"
#include <Arduino.h>


/**
 * Constructor for a SimpleIO device
 * @param pin_number - pin that component is plugged into on the board
 * @return SimpleIO object
*/
SimpleIO::SimpleIO(int pin_number) {
    _pin = pin_number;
    _IO_state = 0;
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}


bool SimpleIO::get_state() {return _IO_state; }

/**
 * Turn on (set to HIGH) the I/O pin.
 */
void SimpleIO::turn_on() {
    _IO_state = 1;
    digitalWrite(_pin, _IO_state);
    return;
}

/**
 * Turn off (set to LOW) the I/O pin.
 */
void SimpleIO::turn_off() {
    _IO_state = 0;
    digitalWrite(_pin, _IO_state);
    return;
}

/**
 * Toggle the state of the I/O pin (HIGH to LOW or LOW to HIGH).
 */
void SimpleIO::toggle() {
    _IO_state = !_IO_state;
    digitalWrite(_pin, _IO_state);
    return;
}

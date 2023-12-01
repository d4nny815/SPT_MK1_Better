#ifndef DigitalInput_h
#define DigitalInput_h
#include <Arduino.h>


/**
 * @brief Class for reading basic digital inputs
*/
class DigitalInput {
    private:
        int _pin;

        bool _btn_state;
        bool _last_btn_state = false;
        unsigned long _cur_time;
        unsigned long _prev_time_check;
        unsigned long _debounce_time_us = 200;
    public:
        DigitalInput(int pin);
        bool read();
};

#endif
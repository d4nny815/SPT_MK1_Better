#ifndef DigitalInput_h
#define DigitalInput_h
#include <Arduino.h>

class DigitalInput {
    private:
        int _pin;
    public:
        DigitalInput(int pin);
        bool read();
};

#endif
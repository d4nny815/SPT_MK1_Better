#ifndef Transducer_h
#define Transducer_h
#include <Arduino.h>
#include <stdlib.h>
class Transducer {
    private:
        int _pin;
        int _value;
        
    public:
        Transducer(int pin);
        u_int32_t get_value_psi();
};


#endif
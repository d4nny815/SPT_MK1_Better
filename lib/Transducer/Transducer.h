#ifndef Transducer_h
#define Transducer_h
#include <Arduino.h>
#include <stdlib.h>
class Transducer {
    private:
        int _pin;
        int _value;
        const int _ducerAmpRange = 16;
        const int _ducerPSIRange = 1000; //NOT TRUE VALUE

        
    public:
        Transducer(int pin);
        u_int32_t get_value_psi();
        u_int32_t voltageToPSI(int ResistorVoltage);
};



#endif


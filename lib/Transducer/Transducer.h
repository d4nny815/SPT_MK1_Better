#ifndef Transducer_h
#define Transducer_h
#include <Arduino.h>
#include <stdlib.h>

class Transducer {
    private:
        int _pin;
        int _value;

        
        const int _ducerAmpRange = 16;
        const int _ducerPSIRange = 1000;
        const int _resistorValue = 1000;
        // start of our ducer range in mAmps
        const int _ducerOffset = 0.004; // This may be the cause of ducer PSI readout error
        
    public:
        Transducer(int pin);
        //u_int32_t get_value_psi();
        u_int32_t voltageToPSI(int ResistorVoltage);
        int ducerPSI;
        int ducer1;
};



#endif


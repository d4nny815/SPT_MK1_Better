#include <stdlib.h>
#include <Arduino.h>
#include "Transducer.h"

//Converting the resistors voltage into a PSI value
u_int32_t Transducer::get_PSI(){
    int ResistorVoltage = analogRead(_pin);
    
    int _ducerAmp;
    int ScaleFactorPSI = _ducerPSIRange/_ducerAmpRange; //a conversion factor with psi per amp 

    _ducerAmp = ResistorVoltage/_resistorValue;
    _ducerPSI = ScaleFactorPSI*(_ducerAmp - _ducerOffset);

    //ducerPSI = map(4, 20, 0, 1000)

    Serial.println(_ducerPSI);
    return _ducerPSI;
}

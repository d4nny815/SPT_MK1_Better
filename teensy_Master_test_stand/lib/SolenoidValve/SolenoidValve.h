#ifndef SoleinoidValve_h
#define SoleinoidValve_h
#include <Arduino.h>

class SoleinoidValve {
    private:
        int _pin;
        bool _IO_state;
        float _pullin_voltage;
        float _holding_voltage;
        bool _is_on;

        unsigned long _current_time;
        unsigned long _previous_time_check;
        unsigned long _pull_in_time_interval = 30;

    public:
        SoleinoidValve(int pin, uint8_t pullin_voltage, uint8_t holding_voltage);
        bool get_state();
        void turn_on();
        void turn_off();

};



#endif

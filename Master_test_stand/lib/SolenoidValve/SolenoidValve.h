#ifndef SoleinoidValve_h
#define SoleinoidValve_h
#include <Arduino.h>

class SoleinoidValve {
    private:
        int _pin;
        bool _IO_state;
        bool _in_pull_in_state;
        uint8_t _pull_in_voltage;
        uint8_t _holding_voltage;

        unsigned long _current_time;
        unsigned long _previous_time_check;
        unsigned long _pull_in_time_interval = 3000;

    public:
        SoleinoidValve(int pin, uint8_t pullin_voltage, uint8_t holding_voltage);
        bool get_state();
        void open();
        void close();

};



#endif

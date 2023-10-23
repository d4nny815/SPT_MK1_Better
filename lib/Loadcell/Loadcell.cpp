#include "Loadcell.h"
#include "Arduino.h"

Loadcell::Loadcell(int dout_pin, int sck_pin, float calibration_factor) {
    _dout_pin = dout_pin;
    _clk_pin = sck_pin;
    _calibration_factor = calibration_factor;
    _loadcell.begin(_dout_pin, _clk_pin);
    _loadcell.set_scale(_calibration_factor);
    _loadcell.tare();
}

double Loadcell::get_value_lbs() {
    double ounces = _loadcell.get_units(5) * 0.035274;
    return ounces;
}




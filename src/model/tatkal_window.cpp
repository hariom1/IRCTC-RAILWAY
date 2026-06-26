#include "irct/tatkal_window.h"

namespace irct {

TatkalWindow::TatkalWindow() : ac_opening_(1000), non_ac_opening_(1100) {}

bool TatkalWindow::is_ac_class(FareClass cls) const {
    return cls == FareClass::ThirdAc || cls == FareClass::SecondAc ||
           cls == FareClass::FirstAc;
}

uint16_t TatkalWindow::opening_time(FareClass cls) const {
    if (is_ac_class(cls)) {
        return ac_opening_;
    }
    return non_ac_opening_;
}

bool TatkalWindow::is_open(FareClass cls, uint16_t current_time) const {
    return current_time >= opening_time(cls);
}

static int packed_to_minutes(uint16_t packed) {
    int hours = packed / 100;
    int mins = packed % 100;
    return hours * 60 + mins;
}

int TatkalWindow::minutes_until_open(FareClass cls,
                                     uint16_t current_time) const {
    int open = packed_to_minutes(opening_time(cls));
    int now = packed_to_minutes(current_time);
    if (now >= open) {
        return 0;
    }
    return open - now;
}

uint32_t TatkalWindow::tatkal_charge(FareClass cls,
                                     uint32_t distance_km) const {
    uint32_t per_km;
    uint32_t minimum;
    uint32_t maximum;
    switch (cls) {
        case FareClass::Second:
            per_km = 1;
            minimum = 1000;
            maximum = 1500;
            break;
        case FareClass::Sleeper:
            per_km = 2;
            minimum = 1000;
            maximum = 2000;
            break;
        case FareClass::ThirdAc:
            per_km = 3;
            minimum = 3000;
            maximum = 4000;
            break;
        case FareClass::SecondAc:
            per_km = 4;
            minimum = 4000;
            maximum = 5000;
            break;
        case FareClass::FirstAc:
            per_km = 5;
            minimum = 5000;
            maximum = 6000;
            break;
        default:
            per_km = 2;
            minimum = 1000;
            maximum = 2000;
            break;
    }
    uint32_t charge = per_km * distance_km;
    if (charge < minimum) {
        charge = minimum;
    }
    if (charge > maximum) {
        charge = maximum;
    }
    return charge;
}

uint32_t TatkalWindow::premium_charge(FareClass cls, uint32_t base_fare) const {
    uint32_t multiplier = is_ac_class(cls) ? 150u : 130u;
    return base_fare * multiplier / 100u;
}

uint16_t TatkalWindow::max_tatkal_passengers() const {
    return 4;
}

const char *TatkalWindow::window_status(FareClass cls,
                                        uint16_t current_time) const {
    if (is_open(cls, current_time)) {
        return "open";
    }
    return "closed";
}

}

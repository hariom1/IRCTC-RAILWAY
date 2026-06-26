#ifndef IRCT_TATKAL_WINDOW_H
#define IRCT_TATKAL_WINDOW_H

#include <cstdint>
#include <string>

#include "irct/fare_table.h"

namespace irct {

class TatkalWindow {
public:
    TatkalWindow();

    uint16_t opening_time(FareClass cls) const;

    bool is_open(FareClass cls, uint16_t current_time) const;

    int minutes_until_open(FareClass cls, uint16_t current_time) const;

    uint32_t tatkal_charge(FareClass cls, uint32_t distance_km) const;

    uint32_t premium_charge(FareClass cls, uint32_t base_fare) const;

    bool is_ac_class(FareClass cls) const;

    uint16_t max_tatkal_passengers() const;

    const char *window_status(FareClass cls, uint16_t current_time) const;

private:
    uint16_t ac_opening_;
    uint16_t non_ac_opening_;
};

}

#endif

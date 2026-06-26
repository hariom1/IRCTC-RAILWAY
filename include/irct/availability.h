#ifndef IRCT_AVAILABILITY_H
#define IRCT_AVAILABILITY_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/fare_table.h"

namespace irct {

enum class Quota {
    General,
    Tatkal,
    Ladies,
    SeniorCitizen,
    Premium
};

struct Coach {
    std::string code;
    FareClass cls;
    uint16_t total_berths;
    uint16_t booked_berths;
};

class Availability {
public:
    Availability();

    void add_coach(const Coach &coach);

    uint16_t open_berths(FareClass cls) const;

    bool book(FareClass cls, uint16_t count);

    void release(FareClass cls, uint16_t count);

    uint16_t quota_limit(Quota quota, FareClass cls) const;

    size_t coach_count() const;

    bool is_available(FareClass cls) const;

private:
    std::vector<Coach> coaches_;
};

}

#endif

#ifndef IRCT_SCHEDULE_H
#define IRCT_SCHEDULE_H

#include <cstdint>
#include <string>

namespace irct {

struct DayTime {
    uint16_t minutes;
};

class Schedule {
public:
    static DayTime from_packed(uint16_t packed);

    static uint16_t to_packed(const DayTime &t);

    static bool is_valid(const DayTime &t);

    static int span_minutes(const DayTime &start, const DayTime &end);

    static std::string format(const DayTime &t);

    static uint16_t add_minutes(uint16_t packed, int delta);

    static int compare(const DayTime &a, const DayTime &b);
};

}

#endif

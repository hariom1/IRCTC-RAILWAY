#include "irct/schedule.h"

#include <cstdio>

namespace irct {

DayTime Schedule::from_packed(uint16_t packed) {
    DayTime t;
    uint16_t hours = static_cast<uint16_t>(packed / 100u);
    uint16_t mins = static_cast<uint16_t>(packed % 100u);
    t.minutes = static_cast<uint16_t>(hours * 60u + mins);
    return t;
}

uint16_t Schedule::to_packed(const DayTime &t) {
    uint16_t hours = static_cast<uint16_t>(t.minutes / 60u);
    uint16_t mins = static_cast<uint16_t>(t.minutes % 60u);
    return static_cast<uint16_t>(hours * 100u + mins);
}

bool Schedule::is_valid(const DayTime &t) {
    return t.minutes < 24u * 60u;
}

int Schedule::span_minutes(const DayTime &start, const DayTime &end) {
    int s = static_cast<int>(start.minutes);
    int e = static_cast<int>(end.minutes);
    if (e >= s) {
        return e - s;
    }
    return (24 * 60 - s) + e;
}

std::string Schedule::format(const DayTime &t) {
    char buf[8];
    uint16_t hours = static_cast<uint16_t>(t.minutes / 60u);
    uint16_t mins = static_cast<uint16_t>(t.minutes % 60u);
    std::snprintf(buf, sizeof(buf), "%02u:%02u", hours, mins);
    return std::string(buf);
}

uint16_t Schedule::add_minutes(uint16_t packed, int delta) {
    DayTime t = from_packed(packed);
    int total = static_cast<int>(t.minutes) + delta;
    total %= 24 * 60;
    if (total < 0) {
        total += 24 * 60;
    }
    DayTime out;
    out.minutes = static_cast<uint16_t>(total);
    return to_packed(out);
}

int Schedule::compare(const DayTime &a, const DayTime &b) {
    if (a.minutes < b.minutes) {
        return -1;
    }
    if (a.minutes > b.minutes) {
        return 1;
    }
    return 0;
}

}

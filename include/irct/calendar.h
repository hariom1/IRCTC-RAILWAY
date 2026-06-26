#ifndef IRCT_CALENDAR_H
#define IRCT_CALENDAR_H

#include <cstdint>
#include <string>

namespace irct {

struct Date {
    uint16_t year;
    uint8_t month;
    uint8_t day;
};

class Calendar {
public:
    Calendar();

    static bool is_leap(uint16_t year);

    static uint8_t days_in_month(uint16_t year, uint8_t month);

    static bool is_valid(const Date &date);

    static int day_of_week(const Date &date);

    static Date add_days(const Date &date, int delta);

    static int64_t to_ordinal(const Date &date);

    static Date from_ordinal(int64_t ordinal);

    static int difference_days(const Date &a, const Date &b);

    static std::string format(const Date &date);

    static const char *weekday_name(int dow);

    bool runs_on(uint8_t running_mask, const Date &date) const;

private:
    static const int month_offsets_[12];
};

}

#endif

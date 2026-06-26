#ifndef IRCT_HOLIDAY_CALENDAR_H
#define IRCT_HOLIDAY_CALENDAR_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/calendar.h"

namespace irct {

struct Holiday {
    Date date;
    std::string name;
    bool surge;
};

class HolidayCalendar {
public:
    HolidayCalendar();

    void add_holiday(const Date &date, const std::string &name, bool surge);

    bool is_holiday(const Date &date) const;

    bool is_surge_day(const Date &date) const;

    bool name_for(const Date &date, std::string *out) const;

    size_t count() const;

    std::vector<Holiday> in_month(uint16_t year, uint8_t month) const;

    uint32_t surge_multiplier(const Date &date) const;

    size_t surge_day_count() const;

private:
    bool same_date(const Date &a, const Date &b) const;

    std::vector<Holiday> holidays_;
};

}

#endif

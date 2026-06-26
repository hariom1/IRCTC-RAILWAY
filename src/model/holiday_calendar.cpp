#include "irct/holiday_calendar.h"

namespace irct {

HolidayCalendar::HolidayCalendar() {}

bool HolidayCalendar::same_date(const Date &a, const Date &b) const {
    return a.year == b.year && a.month == b.month && a.day == b.day;
}

void HolidayCalendar::add_holiday(const Date &date, const std::string &name,
                                  bool surge) {
    Holiday holiday;
    holiday.date = date;
    holiday.name = name;
    holiday.surge = surge;
    holidays_.push_back(holiday);
}

bool HolidayCalendar::is_holiday(const Date &date) const {
    for (size_t i = 0; i < holidays_.size(); ++i) {
        if (same_date(holidays_[i].date, date)) {
            return true;
        }
    }
    return false;
}

bool HolidayCalendar::is_surge_day(const Date &date) const {
    for (size_t i = 0; i < holidays_.size(); ++i) {
        if (same_date(holidays_[i].date, date) && holidays_[i].surge) {
            return true;
        }
    }
    return false;
}

bool HolidayCalendar::name_for(const Date &date, std::string *out) const {
    for (size_t i = 0; i < holidays_.size(); ++i) {
        if (same_date(holidays_[i].date, date)) {
            if (out) {
                *out = holidays_[i].name;
            }
            return true;
        }
    }
    return false;
}

size_t HolidayCalendar::count() const {
    return holidays_.size();
}

std::vector<Holiday> HolidayCalendar::in_month(uint16_t year,
                                               uint8_t month) const {
    std::vector<Holiday> out;
    for (size_t i = 0; i < holidays_.size(); ++i) {
        if (holidays_[i].date.year == year &&
            holidays_[i].date.month == month) {
            out.push_back(holidays_[i]);
        }
    }
    return out;
}

uint32_t HolidayCalendar::surge_multiplier(const Date &date) const {
    if (is_surge_day(date)) {
        return 150;
    }
    if (is_holiday(date)) {
        return 120;
    }
    return 100;
}

size_t HolidayCalendar::surge_day_count() const {
    size_t total = 0;
    for (size_t i = 0; i < holidays_.size(); ++i) {
        if (holidays_[i].surge) {
            ++total;
        }
    }
    return total;
}

}

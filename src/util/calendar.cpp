#include "irct/calendar.h"

#include <cstdio>

namespace irct {

const int Calendar::month_offsets_[12] = {0,   31,  59,  90,  120, 151,
                                          181, 212, 243, 273, 304, 334};

Calendar::Calendar() {}

bool Calendar::is_leap(uint16_t year) {
    if (year % 4 != 0) {
        return false;
    }
    if (year % 100 != 0) {
        return true;
    }
    return year % 400 == 0;
}

uint8_t Calendar::days_in_month(uint16_t year, uint8_t month) {
    if (month < 1 || month > 12) {
        return 0;
    }
    static const uint8_t counts[12] = {31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap(year)) {
        return 29;
    }
    return counts[month - 1];
}

bool Calendar::is_valid(const Date &date) {
    if (date.month < 1 || date.month > 12) {
        return false;
    }
    if (date.day < 1) {
        return false;
    }
    if (date.day > days_in_month(date.year, date.month)) {
        return false;
    }
    return true;
}

int64_t Calendar::to_ordinal(const Date &date) {
    int64_t y = date.year;
    int64_t total = (y - 1) * 365;
    total += (y - 1) / 4;
    total -= (y - 1) / 100;
    total += (y - 1) / 400;
    total += month_offsets_[date.month - 1];
    if (date.month > 2 && is_leap(date.year)) {
        total += 1;
    }
    total += date.day;
    return total;
}

Date Calendar::from_ordinal(int64_t ordinal) {
    Date date;
    int64_t year = 1;
    while (true) {
        int64_t year_len = is_leap(static_cast<uint16_t>(year)) ? 366 : 365;
        if (ordinal <= year_len) {
            break;
        }
        ordinal -= year_len;
        ++year;
    }
    date.year = static_cast<uint16_t>(year);
    uint8_t month = 1;
    while (month <= 12) {
        uint8_t dim = days_in_month(date.year, month);
        if (ordinal <= dim) {
            break;
        }
        ordinal -= dim;
        ++month;
    }
    date.month = month;
    date.day = static_cast<uint8_t>(ordinal);
    return date;
}

Date Calendar::add_days(const Date &date, int delta) {
    int64_t ord = to_ordinal(date) + delta;
    if (ord < 1) {
        ord = 1;
    }
    return from_ordinal(ord);
}

int Calendar::difference_days(const Date &a, const Date &b) {
    return static_cast<int>(to_ordinal(a) - to_ordinal(b));
}

int Calendar::day_of_week(const Date &date) {
    int64_t ord = to_ordinal(date);
    int dow = static_cast<int>(ord % 7);
    return dow;
}

const char *Calendar::weekday_name(int dow) {
    static const char *names[7] = {"Monday",   "Tuesday", "Wednesday",
                                   "Thursday", "Friday",  "Saturday",
                                   "Sunday"};
    int idx = ((dow % 7) + 7) % 7;
    return names[idx];
}

std::string Calendar::format(const Date &date) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04u-%02u-%02u",
                  static_cast<unsigned>(date.year),
                  static_cast<unsigned>(date.month),
                  static_cast<unsigned>(date.day));
    return std::string(buf);
}

bool Calendar::runs_on(uint8_t running_mask, const Date &date) const {
    int dow = day_of_week(date);
    uint8_t bit = static_cast<uint8_t>(1u << (dow % 7));
    return (running_mask & bit) != 0;
}

}

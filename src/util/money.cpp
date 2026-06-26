#include "irct/money.h"

#include <cstdio>
#include <cstdlib>

namespace irct {

Money::Money() : paise_(0) {}

Money::Money(int64_t paise) : paise_(paise) {}

Money Money::from_rupees(int64_t rupees) {
    return Money(rupees * 100);
}

Money Money::from_rupees_paise(int64_t rupees, int64_t paise) {
    return Money(rupees * 100 + paise);
}

int64_t Money::paise() const {
    return paise_;
}

int64_t Money::whole_rupees() const {
    return paise_ / 100;
}

int64_t Money::fractional_paise() const {
    int64_t rem = paise_ % 100;
    if (rem < 0) {
        rem = -rem;
    }
    return rem;
}

Money Money::operator+(const Money &other) const {
    return Money(paise_ + other.paise_);
}

Money Money::operator-(const Money &other) const {
    return Money(paise_ - other.paise_);
}

Money Money::times(int64_t factor) const {
    return Money(paise_ * factor);
}

Money Money::percent(int64_t pct) const {
    return Money((paise_ * pct) / 100);
}

bool Money::is_negative() const {
    return paise_ < 0;
}

bool Money::is_zero() const {
    return paise_ == 0;
}

int Money::compare(const Money &other) const {
    if (paise_ < other.paise_) {
        return -1;
    }
    if (paise_ > other.paise_) {
        return 1;
    }
    return 0;
}

std::string Money::format() const {
    int64_t value = paise_;
    bool negative = value < 0;
    if (negative) {
        value = -value;
    }
    int64_t rupees = value / 100;
    int64_t paise = value % 100;
    char buf[48];
    if (negative) {
        std::snprintf(buf, sizeof(buf), "-Rs %lld.%02lld",
                      static_cast<long long>(rupees),
                      static_cast<long long>(paise));
    } else {
        std::snprintf(buf, sizeof(buf), "Rs %lld.%02lld",
                      static_cast<long long>(rupees),
                      static_cast<long long>(paise));
    }
    return std::string(buf);
}

Money Money::rounded_to_rupee() const {
    int64_t rem = paise_ % 100;
    int64_t base = paise_ - rem;
    if (rem >= 50) {
        base += 100;
    } else if (rem <= -50) {
        base -= 100;
    }
    return Money(base);
}

Money Money::clamp_non_negative() const {
    if (paise_ < 0) {
        return Money(0);
    }
    return Money(paise_);
}

}

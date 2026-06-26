#ifndef IRCT_MONEY_H
#define IRCT_MONEY_H

#include <cstdint>
#include <string>

namespace irct {

class Money {
public:
    Money();

    explicit Money(int64_t paise);

    static Money from_rupees(int64_t rupees);

    static Money from_rupees_paise(int64_t rupees, int64_t paise);

    int64_t paise() const;

    int64_t whole_rupees() const;

    int64_t fractional_paise() const;

    Money operator+(const Money &other) const;

    Money operator-(const Money &other) const;

    Money times(int64_t factor) const;

    Money percent(int64_t pct) const;

    bool is_negative() const;

    bool is_zero() const;

    int compare(const Money &other) const;

    std::string format() const;

    Money rounded_to_rupee() const;

    Money clamp_non_negative() const;

private:
    int64_t paise_;
};

}

#endif

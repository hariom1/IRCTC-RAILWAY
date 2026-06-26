#ifndef IRCT_PASSENGER_RULES_H
#define IRCT_PASSENGER_RULES_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/ticket.h"

namespace irct {

enum class PassengerIssue {
    Ok,
    EmptyName,
    InvalidAge,
    InvalidGender,
    TooManyInGroup,
    InfantWithoutAdult,
    SeniorBerthMismatch
};

struct ValidationOutcome {
    bool valid;
    std::vector<PassengerIssue> issues;
};

class PassengerRules {
public:
    PassengerRules();

    PassengerIssue check_one(const Passenger &p) const;

    ValidationOutcome check_group(const std::vector<Passenger> &group) const;

    bool needs_adult(const Passenger &p) const;

    bool prefers_lower_berth(const Passenger &p) const;

    uint8_t max_group_size() const;

    const char *issue_label(PassengerIssue issue) const;

    size_t count_children(const std::vector<Passenger> &group) const;

private:
    bool valid_gender(char gender) const;

    uint8_t max_group_;
};

}

#endif

#include "irct/passenger_rules.h"

namespace irct {

PassengerRules::PassengerRules() : max_group_(6) {}

bool PassengerRules::valid_gender(char gender) const {
    return gender == 'M' || gender == 'F' || gender == 'T';
}

PassengerIssue PassengerRules::check_one(const Passenger &p) const {
    if (p.name.empty()) {
        return PassengerIssue::EmptyName;
    }
    if (p.age > 125) {
        return PassengerIssue::InvalidAge;
    }
    if (!valid_gender(p.gender)) {
        return PassengerIssue::InvalidGender;
    }
    return PassengerIssue::Ok;
}

bool PassengerRules::needs_adult(const Passenger &p) const {
    return p.age < 5;
}

bool PassengerRules::prefers_lower_berth(const Passenger &p) const {
    if (p.age >= 60) {
        return true;
    }
    if (p.gender == 'F' && p.age >= 45) {
        return true;
    }
    return false;
}

uint8_t PassengerRules::max_group_size() const {
    return max_group_;
}

size_t PassengerRules::count_children(
    const std::vector<Passenger> &group) const {
    size_t children = 0;
    for (size_t i = 0; i < group.size(); ++i) {
        if (group[i].age < 12) {
            ++children;
        }
    }
    return children;
}

ValidationOutcome PassengerRules::check_group(
    const std::vector<Passenger> &group) const {
    ValidationOutcome outcome;
    outcome.valid = true;

    if (group.size() > max_group_) {
        outcome.issues.push_back(PassengerIssue::TooManyInGroup);
        outcome.valid = false;
    }

    bool has_adult = false;
    bool has_infant = false;
    for (size_t i = 0; i < group.size(); ++i) {
        PassengerIssue issue = check_one(group[i]);
        if (issue != PassengerIssue::Ok) {
            outcome.issues.push_back(issue);
            outcome.valid = false;
        }
        if (group[i].age >= 18) {
            has_adult = true;
        }
        if (needs_adult(group[i])) {
            has_infant = true;
        }
    }

    if (has_infant && !has_adult) {
        outcome.issues.push_back(PassengerIssue::InfantWithoutAdult);
        outcome.valid = false;
    }

    return outcome;
}

const char *PassengerRules::issue_label(PassengerIssue issue) const {
    switch (issue) {
        case PassengerIssue::Ok:
            return "ok";
        case PassengerIssue::EmptyName:
            return "empty name";
        case PassengerIssue::InvalidAge:
            return "invalid age";
        case PassengerIssue::InvalidGender:
            return "invalid gender";
        case PassengerIssue::TooManyInGroup:
            return "too many in group";
        case PassengerIssue::InfantWithoutAdult:
            return "infant without adult";
        case PassengerIssue::SeniorBerthMismatch:
            return "senior berth mismatch";
    }
    return "unknown";
}

}

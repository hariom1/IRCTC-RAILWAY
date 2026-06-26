#include "irct/concession.h"
namespace irct {
Concession::Concession() {}
uint32_t Concession::percent(ConcessionType type) const {
    switch (type) {
        case ConcessionType::None:
            return 0;
        case ConcessionType::SeniorMale:
            return 40;
        case ConcessionType::SeniorFemale:
            return 50;
        case ConcessionType::Student:
            return 25;
        case ConcessionType::Disabled:
            return 75;
        case ConcessionType::Patient:
            return 50;
    }
    return 0;
}
uint32_t Concession::round_paise(uint32_t value) const {
    uint32_t rem = value % 500u;
    if (rem == 0) {
        return value;
    }
    return value - rem + 500u;
}
uint32_t Concession::apply(uint32_t base_paise, ConcessionType type) const {
    uint32_t pct = percent(type);
    uint32_t discount = (base_paise * pct) / 100u;
    uint32_t net = base_paise - discount;
    return round_paise(net);
}
bool Concession::eligible_by_age(ConcessionType type, uint8_t age) const {
    return age >= min_age(type);
}
uint32_t Concession::min_age(ConcessionType type) const {
    switch (type) {
        case ConcessionType::SeniorMale:
            return 60;
        case ConcessionType::SeniorFemale:
            return 58;
        case ConcessionType::Student:
            return 5;
        case ConcessionType::Disabled:
            return 0;
        case ConcessionType::Patient:
            return 0;
        case ConcessionType::None:
            return 0;
    }
    return 0;
}
const char *Concession::label(ConcessionType type) const {
    switch (type) {
        case ConcessionType::None:
            return "none";
        case ConcessionType::SeniorMale:
            return "senior male";
        case ConcessionType::SeniorFemale:
            return "senior female";
        case ConcessionType::Student:
            return "student";
        case ConcessionType::Disabled:
            return "disabled";
        case ConcessionType::Patient:
            return "patient";
    }
    return "unknown";
}
}

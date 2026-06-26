#include "irct/fare_rules.h"
namespace irct {
FareRules::FareRules() {}
void FareRules::add_rule(const FareRule &rule) {
    rules_.push_back(rule);
}
bool FareRules::applies(const FareRule &rule, uint32_t km, uint8_t weekday) const {
    if (km < rule.min_km || km > rule.max_km) {
        return false;
    }
    uint8_t bit = static_cast<uint8_t>(1u << (weekday % 7));
    return (rule.weekday_mask & bit) != 0;
}
uint32_t FareRules::surcharge(uint32_t distance_km, uint8_t weekday) const {
    uint32_t total = 0;
    for (size_t i = 0; i < rules_.size(); ++i) {
        if (applies(rules_[i], distance_km, weekday)) {
            total += rules_[i].surcharge_paise;
        }
    }
    return total;
}
size_t FareRules::rule_count() const {
    return rules_.size();
}
uint32_t FareRules::peak_multiplier(uint8_t weekday) const {
    if (weekday == 5 || weekday == 6) {
        return 120;
    }
    return 100;
}
void FareRules::load_standard() {
    FareRule a;
    a.min_km = 0;
    a.max_km = 200;
    a.surcharge_paise = 0;
    a.weekday_mask = 0x7F;
    add_rule(a);
    FareRule b;
    b.min_km = 201;
    b.max_km = 800;
    b.surcharge_paise = 2000;
    b.weekday_mask = 0x7F;
    add_rule(b);
    FareRule c;
    c.min_km = 801;
    c.max_km = 5000;
    c.surcharge_paise = 5000;
    c.weekday_mask = 0x60;
    add_rule(c);
}
void FareRules::clear() {
    rules_.clear();
}
}

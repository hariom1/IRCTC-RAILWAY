#ifndef IRCT_FARE_RULES_H
#define IRCT_FARE_RULES_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
struct FareRule {
    uint32_t min_km;
    uint32_t max_km;
    uint32_t surcharge_paise;
    uint8_t weekday_mask;
};
class FareRules {
public:
    FareRules();
    void add_rule(const FareRule &rule);
    uint32_t surcharge(uint32_t distance_km, uint8_t weekday) const;
    bool applies(const FareRule &rule, uint32_t km, uint8_t weekday) const;
    size_t rule_count() const;
    void load_standard();
    uint32_t peak_multiplier(uint8_t weekday) const;
    void clear();
private:
    std::vector<FareRule> rules_;
};
}
#endif

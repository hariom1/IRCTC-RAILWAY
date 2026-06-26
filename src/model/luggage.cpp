#include "irct/luggage.h"
namespace irct {
LuggageManager::LuggageManager() {}
uint32_t LuggageManager::free_allowance_grams(LuggageClass cls) const {
    switch (cls) {
        case LuggageClass::Handbag:
            return 10000;
        case LuggageClass::Suitcase:
            return 40000;
        case LuggageClass::Trunk:
            return 50000;
        case LuggageClass::Bicycle:
            return 0;
        case LuggageClass::Parcel:
            return 0;
    }
    return 0;
}
uint32_t LuggageManager::rate_per_kg_paise(LuggageClass cls) const {
    switch (cls) {
        case LuggageClass::Handbag:
            return 200;
        case LuggageClass::Suitcase:
            return 300;
        case LuggageClass::Trunk:
            return 350;
        case LuggageClass::Bicycle:
            return 500;
        case LuggageClass::Parcel:
            return 600;
    }
    return 300;
}
uint32_t LuggageManager::max_weight_grams(LuggageClass cls) const {
    switch (cls) {
        case LuggageClass::Handbag:
            return 15000;
        case LuggageClass::Suitcase:
            return 70000;
        case LuggageClass::Trunk:
            return 80000;
        case LuggageClass::Bicycle:
            return 30000;
        case LuggageClass::Parcel:
            return 100000;
    }
    return 50000;
}
bool LuggageManager::exceeds_limit(LuggageClass cls,
                                   uint32_t weight_grams) const {
    return weight_grams > max_weight_grams(cls);
}
uint32_t LuggageManager::compute_charge(LuggageClass cls,
                                        uint32_t weight_grams) const {
    uint32_t allowance = free_allowance_grams(cls);
    if (weight_grams <= allowance) {
        return 0;
    }
    uint32_t billable = weight_grams - allowance;
    uint32_t kg = billable / 1000u;
    if (billable % 1000u != 0) {
        kg += 1;
    }
    return kg * rate_per_kg_paise(cls);
}
bool LuggageManager::book(const std::string &pnr, LuggageClass cls,
                          uint32_t weight_grams) {
    if (exceeds_limit(cls, weight_grams)) {
        return false;
    }
    LuggageItem item;
    item.pnr = pnr;
    item.type = cls;
    item.weight_grams = weight_grams;
    item.charge_paise = compute_charge(cls, weight_grams);
    items_.push_back(item);
    return true;
}
uint32_t LuggageManager::total_weight() const {
    uint32_t total = 0;
    for (size_t i = 0; i < items_.size(); ++i) {
        total += items_[i].weight_grams;
    }
    return total;
}
uint32_t LuggageManager::total_revenue() const {
    uint32_t total = 0;
    for (size_t i = 0; i < items_.size(); ++i) {
        total += items_[i].charge_paise;
    }
    return total;
}
size_t LuggageManager::item_count() const {
    return items_.size();
}
uint32_t LuggageManager::weight_for_pnr(const std::string &pnr) const {
    uint32_t total = 0;
    for (size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].pnr == pnr) {
            total += items_[i].weight_grams;
        }
    }
    return total;
}
const char *LuggageManager::class_label(LuggageClass cls) const {
    switch (cls) {
        case LuggageClass::Handbag:
            return "handbag";
        case LuggageClass::Suitcase:
            return "suitcase";
        case LuggageClass::Trunk:
            return "trunk";
        case LuggageClass::Bicycle:
            return "bicycle";
        case LuggageClass::Parcel:
            return "parcel";
    }
    return "unknown";
}
}

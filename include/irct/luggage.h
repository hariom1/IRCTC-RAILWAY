#ifndef IRCT_LUGGAGE_H
#define IRCT_LUGGAGE_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
enum class LuggageClass {
    Handbag,
    Suitcase,
    Trunk,
    Bicycle,
    Parcel
};
struct LuggageItem {
    std::string pnr;
    LuggageClass type;
    uint32_t weight_grams;
    uint32_t charge_paise;
};
class LuggageManager {
public:
    LuggageManager();
    uint32_t free_allowance_grams(LuggageClass cls) const;
    uint32_t rate_per_kg_paise(LuggageClass cls) const;
    uint32_t compute_charge(LuggageClass cls, uint32_t weight_grams) const;
    bool book(const std::string &pnr, LuggageClass cls, uint32_t weight_grams);
    uint32_t total_weight() const;
    uint32_t total_revenue() const;
    size_t item_count() const;
    bool exceeds_limit(LuggageClass cls, uint32_t weight_grams) const;
    const char *class_label(LuggageClass cls) const;
    uint32_t weight_for_pnr(const std::string &pnr) const;
private:
    uint32_t max_weight_grams(LuggageClass cls) const;
    std::vector<LuggageItem> items_;
};
}
#endif

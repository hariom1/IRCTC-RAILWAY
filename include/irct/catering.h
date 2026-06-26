#ifndef IRCT_CATERING_H
#define IRCT_CATERING_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
enum class MealType {
    None,
    Veg,
    NonVeg,
    Jain,
    Vegan,
    Diabetic
};
struct MealOrder {
    std::string pnr;
    MealType type;
    uint16_t quantity;
    uint32_t price_paise;
};
class CateringManager {
public:
    CateringManager();
    void load_menu();
    uint32_t price_of(MealType type) const;
    bool place_order(const std::string &pnr, MealType type, uint16_t quantity);
    uint32_t revenue() const;
    size_t order_count() const;
    uint16_t quantity_of(MealType type) const;
    bool cancel_order(const std::string &pnr);
    const char *meal_label(MealType type) const;
    bool is_available(MealType type) const;
    uint32_t total_for_pnr(const std::string &pnr) const;
private:
    struct MenuItem {
        MealType type;
        uint32_t price_paise;
        bool available;
    };
    const MenuItem *find_menu(MealType type) const;
    std::vector<MenuItem> menu_;
    std::vector<MealOrder> orders_;
};
}
#endif

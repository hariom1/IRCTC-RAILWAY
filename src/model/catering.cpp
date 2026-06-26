#include "irct/catering.h"
namespace irct {
CateringManager::CateringManager() {}
void CateringManager::load_menu() {
    menu_.clear();
    MenuItem veg;
    veg.type = MealType::Veg;
    veg.price_paise = 5000;
    veg.available = true;
    menu_.push_back(veg);
    MenuItem nonveg;
    nonveg.type = MealType::NonVeg;
    nonveg.price_paise = 7000;
    nonveg.available = true;
    menu_.push_back(nonveg);
    MenuItem jain;
    jain.type = MealType::Jain;
    jain.price_paise = 5500;
    jain.available = true;
    menu_.push_back(jain);
    MenuItem vegan;
    vegan.type = MealType::Vegan;
    vegan.price_paise = 6000;
    vegan.available = true;
    menu_.push_back(vegan);
    MenuItem diabetic;
    diabetic.type = MealType::Diabetic;
    diabetic.price_paise = 6500;
    diabetic.available = false;
    menu_.push_back(diabetic);
}
const CateringManager::MenuItem *CateringManager::find_menu(
    MealType type) const {
    for (size_t i = 0; i < menu_.size(); ++i) {
        if (menu_[i].type == type) {
            return &menu_[i];
        }
    }
    return nullptr;
}
uint32_t CateringManager::price_of(MealType type) const {
    const MenuItem *item = find_menu(type);
    if (!item) {
        return 0;
    }
    return item->price_paise;
}
bool CateringManager::is_available(MealType type) const {
    const MenuItem *item = find_menu(type);
    if (!item) {
        return false;
    }
    return item->available;
}
bool CateringManager::place_order(const std::string &pnr, MealType type,
                                  uint16_t quantity) {
    if (!is_available(type) || quantity == 0) {
        return false;
    }
    MealOrder order;
    order.pnr = pnr;
    order.type = type;
    order.quantity = quantity;
    order.price_paise = price_of(type) * quantity;
    orders_.push_back(order);
    return true;
}
uint32_t CateringManager::revenue() const {
    uint32_t total = 0;
    for (size_t i = 0; i < orders_.size(); ++i) {
        total += orders_[i].price_paise;
    }
    return total;
}
size_t CateringManager::order_count() const {
    return orders_.size();
}
uint16_t CateringManager::quantity_of(MealType type) const {
    uint32_t total = 0;
    for (size_t i = 0; i < orders_.size(); ++i) {
        if (orders_[i].type == type) {
            total += orders_[i].quantity;
        }
    }
    if (total > 0xFFFFu) {
        return 0xFFFFu;
    }
    return static_cast<uint16_t>(total);
}
bool CateringManager::cancel_order(const std::string &pnr) {
    for (size_t i = 0; i < orders_.size(); ++i) {
        if (orders_[i].pnr == pnr) {
            orders_.erase(orders_.begin() + static_cast<long>(i));
            return true;
        }
    }
    return false;
}
uint32_t CateringManager::total_for_pnr(const std::string &pnr) const {
    uint32_t total = 0;
    for (size_t i = 0; i < orders_.size(); ++i) {
        if (orders_[i].pnr == pnr) {
            total += orders_[i].price_paise;
        }
    }
    return total;
}
const char *CateringManager::meal_label(MealType type) const {
    switch (type) {
        case MealType::None:
            return "none";
        case MealType::Veg:
            return "vegetarian";
        case MealType::NonVeg:
            return "non vegetarian";
        case MealType::Jain:
            return "jain";
        case MealType::Vegan:
            return "vegan";
        case MealType::Diabetic:
            return "diabetic";
    }
    return "unknown";
}
}

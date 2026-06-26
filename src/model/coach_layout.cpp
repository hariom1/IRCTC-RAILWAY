#include "irct/coach_layout.h"
namespace irct {
CoachLayout::CoachLayout(const std::string &code) : code_(code) {}
void CoachLayout::generate_sleeper(uint16_t bays) {
    berths_.clear();
    uint16_t number = 1;
    for (uint16_t bay = 0; bay < bays; ++bay) {
        BerthType pattern[8] = {
            BerthType::Lower, BerthType::Middle, BerthType::Upper,
            BerthType::Lower, BerthType::Middle, BerthType::Upper,
            BerthType::SideLower, BerthType::SideUpper};
        for (int i = 0; i < 8; ++i) {
            Berth b;
            b.number = number;
            b.type = pattern[i];
            b.occupied = false;
            berths_.push_back(b);
            ++number;
        }
    }
}
BerthType CoachLayout::berth_type(uint16_t number) const {
    for (size_t i = 0; i < berths_.size(); ++i) {
        if (berths_[i].number == number) {
            return berths_[i].type;
        }
    }
    return BerthType::Lower;
}
bool CoachLayout::allocate(uint16_t number) {
    for (size_t i = 0; i < berths_.size(); ++i) {
        if (berths_[i].number == number) {
            if (berths_[i].occupied) {
                return false;
            }
            berths_[i].occupied = true;
            return true;
        }
    }
    return false;
}
void CoachLayout::free(uint16_t number) {
    for (size_t i = 0; i < berths_.size(); ++i) {
        if (berths_[i].number == number) {
            berths_[i].occupied = false;
            return;
        }
    }
}
uint16_t CoachLayout::first_available(BerthType preferred) const {
    for (size_t i = 0; i < berths_.size(); ++i) {
        if (!berths_[i].occupied && berths_[i].type == preferred) {
            return berths_[i].number;
        }
    }
    for (size_t i = 0; i < berths_.size(); ++i) {
        if (!berths_[i].occupied) {
            return berths_[i].number;
        }
    }
    return 0;
}
size_t CoachLayout::total() const { return berths_.size(); }
size_t CoachLayout::occupied() const {
    size_t n = 0;
    for (size_t i = 0; i < berths_.size(); ++i) {
        if (berths_[i].occupied) {
            ++n;
        }
    }
    return n;
}
const std::string &CoachLayout::code() const { return code_; }
}

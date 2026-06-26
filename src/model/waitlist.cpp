#include "irct/waitlist.h"
namespace irct {
Waitlist::Waitlist() {}
void Waitlist::enqueue(const std::string &pnr, uint8_t passengers) {
    WaitlistItem item;
    item.pnr = pnr;
    item.passengers = passengers;
    item.position = static_cast<uint32_t>(items_.size() + 1);
    items_.push_back(item);
}
bool Waitlist::promote(uint32_t freed_berths) {
    bool any = false;
    while (freed_berths > 0 && !items_.empty()) {
        WaitlistItem &front = items_.front();
        if (front.passengers > freed_berths) {
            break;
        }
        freed_berths -= front.passengers;
        confirmed_.push_back(front.pnr);
        items_.erase(items_.begin());
        any = true;
    }
    renumber();
    return any;
}
uint32_t Waitlist::position_of(const std::string &pnr) const {
    for (size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].pnr == pnr) {
            return items_[i].position;
        }
    }
    return 0;
}
size_t Waitlist::pending() const {
    return items_.size();
}
size_t Waitlist::confirmed() const {
    return confirmed_.size();
}
bool Waitlist::remove(const std::string &pnr) {
    for (size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].pnr == pnr) {
            items_.erase(items_.begin() + static_cast<long>(i));
            renumber();
            return true;
        }
    }
    return false;
}
void Waitlist::renumber() {
    for (size_t i = 0; i < items_.size(); ++i) {
        items_[i].position = static_cast<uint32_t>(i + 1);
    }
}
uint32_t Waitlist::total_waiting_passengers() const {
    uint32_t total = 0;
    for (size_t i = 0; i < items_.size(); ++i) {
        total += items_[i].passengers;
    }
    return total;
}
}

#include "irct/availability.h"

namespace irct {

Availability::Availability() {}

void Availability::add_coach(const Coach &coach) {
    coaches_.push_back(coach);
}

uint16_t Availability::open_berths(FareClass cls) const {
    uint32_t open = 0;
    for (size_t i = 0; i < coaches_.size(); ++i) {
        if (coaches_[i].cls != cls) {
            continue;
        }
        if (coaches_[i].booked_berths < coaches_[i].total_berths) {
            open += static_cast<uint32_t>(coaches_[i].total_berths -
                                          coaches_[i].booked_berths);
        }
    }
    if (open > 0xFFFFu) {
        return 0xFFFFu;
    }
    return static_cast<uint16_t>(open);
}

bool Availability::book(FareClass cls, uint16_t count) {
    if (open_berths(cls) < count) {
        return false;
    }
    uint16_t remaining = count;
    for (size_t i = 0; i < coaches_.size() && remaining > 0; ++i) {
        if (coaches_[i].cls != cls) {
            continue;
        }
        uint16_t free_here = 0;
        if (coaches_[i].booked_berths < coaches_[i].total_berths) {
            free_here = static_cast<uint16_t>(coaches_[i].total_berths -
                                              coaches_[i].booked_berths);
        }
        uint16_t take = free_here < remaining ? free_here : remaining;
        coaches_[i].booked_berths =
            static_cast<uint16_t>(coaches_[i].booked_berths + take);
        remaining = static_cast<uint16_t>(remaining - take);
    }
    return true;
}

void Availability::release(FareClass cls, uint16_t count) {
    uint16_t remaining = count;
    for (size_t i = 0; i < coaches_.size() && remaining > 0; ++i) {
        if (coaches_[i].cls != cls) {
            continue;
        }
        uint16_t take = coaches_[i].booked_berths < remaining
                            ? coaches_[i].booked_berths
                            : remaining;
        coaches_[i].booked_berths =
            static_cast<uint16_t>(coaches_[i].booked_berths - take);
        remaining = static_cast<uint16_t>(remaining - take);
    }
}

uint16_t Availability::quota_limit(Quota quota, FareClass cls) const {
    uint16_t total = 0;
    for (size_t i = 0; i < coaches_.size(); ++i) {
        if (coaches_[i].cls == cls) {
            total = static_cast<uint16_t>(total + coaches_[i].total_berths);
        }
    }
    uint32_t share = total;
    switch (quota) {
        case Quota::General:
            share = total;
            break;
        case Quota::Tatkal:
            share = (total * 30u) / 100u;
            break;
        case Quota::Ladies:
            share = (total * 6u) / 100u;
            break;
        case Quota::SeniorCitizen:
            share = (total * 10u) / 100u;
            break;
        case Quota::Premium:
            share = (total * 15u) / 100u;
            break;
    }
    return static_cast<uint16_t>(share);
}

size_t Availability::coach_count() const {
    return coaches_.size();
}

bool Availability::is_available(FareClass cls) const {
    return open_berths(cls) > 0;
}

}

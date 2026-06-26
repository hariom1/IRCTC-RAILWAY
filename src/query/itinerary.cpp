#include "irct/itinerary.h"

#include "irct/schedule.h"

namespace irct {

Itinerary::Itinerary() {
    empty_.train_number = 0;
    empty_.from_station = 0;
    empty_.to_station = 0;
    empty_.departure = 0;
    empty_.arrival = 0;
    empty_.day_offset = 0;
    empty_.distance_km = 0;
}

void Itinerary::add_leg(const ItineraryLeg &leg) {
    legs_.push_back(leg);
}

bool Itinerary::is_connected() const {
    for (size_t i = 1; i < legs_.size(); ++i) {
        if (legs_[i - 1].to_station != legs_[i].from_station) {
            return false;
        }
    }
    return true;
}

int Itinerary::leg_minutes(const ItineraryLeg &leg) const {
    DayTime dep = Schedule::from_packed(leg.departure);
    DayTime arr = Schedule::from_packed(leg.arrival);
    int minutes = static_cast<int>(arr.minutes) - static_cast<int>(dep.minutes);
    minutes += leg.day_offset * 24 * 60;
    if (minutes < 0) {
        minutes += 24 * 60;
    }
    return minutes;
}

int Itinerary::total_travel_minutes() const {
    int total = 0;
    for (size_t i = 0; i < legs_.size(); ++i) {
        total += leg_minutes(legs_[i]);
    }
    return total;
}

int Itinerary::total_layover_minutes() const {
    int total = 0;
    for (size_t i = 1; i < legs_.size(); ++i) {
        DayTime prev_arr = Schedule::from_packed(legs_[i - 1].arrival);
        DayTime next_dep = Schedule::from_packed(legs_[i].departure);
        int gap = static_cast<int>(next_dep.minutes) -
                  static_cast<int>(prev_arr.minutes);
        if (gap < 0) {
            gap += 24 * 60;
        }
        total += gap;
    }
    return total;
}

uint32_t Itinerary::total_distance() const {
    uint32_t total = 0;
    for (size_t i = 0; i < legs_.size(); ++i) {
        total += legs_[i].distance_km;
    }
    return total;
}

size_t Itinerary::leg_count() const {
    return legs_.size();
}

size_t Itinerary::transfer_count() const {
    if (legs_.empty()) {
        return 0;
    }
    return legs_.size() - 1;
}

bool Itinerary::has_tight_connection(int minimum_minutes) const {
    for (size_t i = 1; i < legs_.size(); ++i) {
        DayTime prev_arr = Schedule::from_packed(legs_[i - 1].arrival);
        DayTime next_dep = Schedule::from_packed(legs_[i].departure);
        int gap = static_cast<int>(next_dep.minutes) -
                  static_cast<int>(prev_arr.minutes);
        if (gap < 0) {
            gap += 24 * 60;
        }
        if (gap < minimum_minutes) {
            return true;
        }
    }
    return false;
}

const ItineraryLeg &Itinerary::at(size_t i) const {
    if (i >= legs_.size()) {
        return empty_;
    }
    return legs_[i];
}

uint32_t Itinerary::origin() const {
    if (legs_.empty()) {
        return 0;
    }
    return legs_.front().from_station;
}

uint32_t Itinerary::destination() const {
    if (legs_.empty()) {
        return 0;
    }
    return legs_.back().to_station;
}

}

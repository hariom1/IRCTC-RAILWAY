#include "irct/amenities.h"

namespace irct {

StationAmenities::StationAmenities() {}

uint16_t StationAmenities::bit_of(Amenity amenity) {
    return static_cast<uint16_t>(1u << static_cast<unsigned>(amenity));
}

StationAmenities::Record *StationAmenities::find(uint32_t station) {
    for (size_t i = 0; i < records_.size(); ++i) {
        if (records_[i].station == station) {
            return &records_[i];
        }
    }
    return nullptr;
}

const StationAmenities::Record *StationAmenities::find(uint32_t station) const {
    for (size_t i = 0; i < records_.size(); ++i) {
        if (records_[i].station == station) {
            return &records_[i];
        }
    }
    return nullptr;
}

void StationAmenities::set(uint32_t station, Amenity amenity, bool available) {
    Record *rec = find(station);
    if (!rec) {
        Record fresh;
        fresh.station = station;
        fresh.mask = 0;
        records_.push_back(fresh);
        rec = &records_.back();
    }
    uint16_t bit = bit_of(amenity);
    if (available) {
        rec->mask = static_cast<uint16_t>(rec->mask | bit);
    } else {
        rec->mask = static_cast<uint16_t>(rec->mask & ~bit);
    }
}

bool StationAmenities::has(uint32_t station, Amenity amenity) const {
    const Record *rec = find(station);
    if (!rec) {
        return false;
    }
    return (rec->mask & bit_of(amenity)) != 0;
}

size_t StationAmenities::count_for(uint32_t station) const {
    const Record *rec = find(station);
    if (!rec) {
        return 0;
    }
    size_t total = 0;
    for (unsigned i = 0; i < 16; ++i) {
        if (rec->mask & (1u << i)) {
            ++total;
        }
    }
    return total;
}

std::vector<Amenity> StationAmenities::list_for(uint32_t station) const {
    std::vector<Amenity> out;
    const Record *rec = find(station);
    if (!rec) {
        return out;
    }
    for (unsigned i = 0; i <= static_cast<unsigned>(Amenity::DrinkingWater);
         ++i) {
        if (rec->mask & (1u << i)) {
            out.push_back(static_cast<Amenity>(i));
        }
    }
    return out;
}

uint32_t StationAmenities::score(uint32_t station) const {
    return static_cast<uint32_t>(count_for(station) * 10u);
}

bool StationAmenities::is_accessible(uint32_t station) const {
    return has(station, Amenity::Wheelchair) && has(station, Amenity::Restroom);
}

const char *StationAmenities::amenity_name(Amenity amenity) const {
    switch (amenity) {
        case Amenity::WaitingRoom:
            return "waiting room";
        case Amenity::Restroom:
            return "restroom";
        case Amenity::FoodStall:
            return "food stall";
        case Amenity::Wifi:
            return "wifi";
        case Amenity::Parking:
            return "parking";
        case Amenity::Wheelchair:
            return "wheelchair access";
        case Amenity::Atm:
            return "atm";
        case Amenity::Cloakroom:
            return "cloakroom";
        case Amenity::Escalator:
            return "escalator";
        case Amenity::DrinkingWater:
            return "drinking water";
    }
    return "unknown";
}

}

#ifndef IRCT_AMENITIES_H
#define IRCT_AMENITIES_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

enum class Amenity {
    WaitingRoom,
    Restroom,
    FoodStall,
    Wifi,
    Parking,
    Wheelchair,
    Atm,
    Cloakroom,
    Escalator,
    DrinkingWater
};

class StationAmenities {
public:
    StationAmenities();

    void set(uint32_t station, Amenity amenity, bool available);

    bool has(uint32_t station, Amenity amenity) const;

    size_t count_for(uint32_t station) const;

    std::vector<Amenity> list_for(uint32_t station) const;

    uint32_t score(uint32_t station) const;

    const char *amenity_name(Amenity amenity) const;

    bool is_accessible(uint32_t station) const;

private:
    struct Record {
        uint32_t station;
        uint16_t mask;
    };

    Record *find(uint32_t station);

    const Record *find(uint32_t station) const;

    static uint16_t bit_of(Amenity amenity);

    std::vector<Record> records_;
};

}

#endif

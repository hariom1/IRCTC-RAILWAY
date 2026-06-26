#ifndef IRCT_ITINERARY_H
#define IRCT_ITINERARY_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

struct ItineraryLeg {
    uint32_t train_number;
    uint32_t from_station;
    uint32_t to_station;
    uint16_t departure;
    uint16_t arrival;
    uint8_t day_offset;
    uint32_t distance_km;
};

class Itinerary {
public:
    Itinerary();

    void add_leg(const ItineraryLeg &leg);

    bool is_connected() const;

    int total_travel_minutes() const;

    int total_layover_minutes() const;

    uint32_t total_distance() const;

    size_t leg_count() const;

    size_t transfer_count() const;

    bool has_tight_connection(int minimum_minutes) const;

    const ItineraryLeg &at(size_t i) const;

    uint32_t origin() const;

    uint32_t destination() const;

private:
    int leg_minutes(const ItineraryLeg &leg) const;

    std::vector<ItineraryLeg> legs_;
    ItineraryLeg empty_;
};

}

#endif

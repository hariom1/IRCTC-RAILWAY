#ifndef IRCT_BOOKING_ENGINE_H
#define IRCT_BOOKING_ENGINE_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/availability.h"
#include "irct/concession.h"
#include "irct/fare_rules.h"
#include "irct/fare_table.h"
#include "irct/ticket.h"
#include "irct/waitlist.h"

namespace irct {

struct BookingRequest {
    uint32_t train_number;
    uint32_t from_station;
    uint32_t to_station;
    uint32_t distance_km;
    FareClass travel_class;
    uint8_t weekday;
    std::vector<Passenger> passengers;
    std::vector<ConcessionType> concessions;
};

struct BookingResult {
    bool accepted;
    bool waitlisted;
    std::string pnr;
    uint32_t fare_paise;
    uint32_t waitlist_position;
    std::string message;
};

class BookingEngine {
public:
    BookingEngine();

    void set_fare_table(const FareTable &table);

    void set_fare_rules(const FareRules &rules);

    void attach_availability(Availability *availability);

    BookingResult book(const BookingRequest &request);

    bool cancel(const std::string &pnr, uint32_t freed_berths);

    size_t total_booked() const;

    size_t total_waitlisted() const;

    uint32_t quote(const BookingRequest &request) const;

    std::string next_pnr();

private:
    uint32_t base_fare_for(const BookingRequest &request) const;

    uint32_t apply_concession(uint32_t base, ConcessionType type) const;

    bool seats_available(const BookingRequest &request) const;

    FareTable fares_;
    FareRules rules_;
    Concession concession_;
    Availability *availability_;
    Waitlist waitlist_;
    uint64_t pnr_counter_;
    size_t booked_;
};

}

#endif

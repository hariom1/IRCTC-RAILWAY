#ifndef IRCT_TICKET_H
#define IRCT_TICKET_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/fare_table.h"

namespace irct {

enum class BookingStatus {
    Confirmed,
    RacWaiting,
    Waitlisted,
    Cancelled
};

struct Passenger {
    std::string name;
    uint8_t age;
    char gender;
    std::string seat;
    BookingStatus status;
};

class Ticket {
public:
    Ticket(const std::string &pnr, uint32_t train_number);

    const std::string &pnr() const;

    uint32_t train_number() const;

    void set_journey(uint32_t from_station, uint32_t to_station,
                     uint32_t distance_km);

    void set_class(FareClass cls);

    void add_passenger(const Passenger &p);

    size_t passenger_count() const;

    const Passenger &passenger(size_t i) const;

    uint32_t fare_total(const FareTable &table) const;

    size_t confirmed_count() const;

    std::string summary() const;

private:
    std::string pnr_;
    uint32_t train_number_;
    uint32_t from_station_;
    uint32_t to_station_;
    uint32_t distance_km_;
    FareClass class_;
    std::vector<Passenger> passengers_;
    Passenger empty_;
};

}

#endif

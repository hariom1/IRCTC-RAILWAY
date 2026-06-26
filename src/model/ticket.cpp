#include "irct/ticket.h"

#include <cstdio>

namespace irct {

Ticket::Ticket(const std::string &pnr, uint32_t train_number)
    : pnr_(pnr),
      train_number_(train_number),
      from_station_(0),
      to_station_(0),
      distance_km_(0),
      class_(FareClass::Sleeper) {
    empty_.name = "";
    empty_.age = 0;
    empty_.gender = 'U';
    empty_.status = BookingStatus::Cancelled;
}

const std::string &Ticket::pnr() const {
    return pnr_;
}

uint32_t Ticket::train_number() const {
    return train_number_;
}

void Ticket::set_journey(uint32_t from_station, uint32_t to_station,
                         uint32_t distance_km) {
    from_station_ = from_station;
    to_station_ = to_station;
    distance_km_ = distance_km;
}

void Ticket::set_class(FareClass cls) {
    class_ = cls;
}

void Ticket::add_passenger(const Passenger &p) {
    passengers_.push_back(p);
}

size_t Ticket::passenger_count() const {
    return passengers_.size();
}

const Passenger &Ticket::passenger(size_t i) const {
    if (i >= passengers_.size()) {
        return empty_;
    }
    return passengers_[i];
}

uint32_t Ticket::fare_total(const FareTable &table) const {
    uint32_t per_head = table.total(class_, distance_km_);
    uint32_t chargeable = 0;
    for (size_t i = 0; i < passengers_.size(); ++i) {
        if (passengers_[i].status != BookingStatus::Cancelled) {
            ++chargeable;
        }
    }
    return per_head * chargeable;
}

size_t Ticket::confirmed_count() const {
    size_t total = 0;
    for (size_t i = 0; i < passengers_.size(); ++i) {
        if (passengers_[i].status == BookingStatus::Confirmed) {
            ++total;
        }
    }
    return total;
}

std::string Ticket::summary() const {
    char buf[128];
    std::snprintf(buf, sizeof(buf),
                  "PNR %s train %u from %u to %u dist %u km pax %zu",
                  pnr_.c_str(), train_number_, from_station_, to_station_,
                  distance_km_, passengers_.size());
    return std::string(buf);
}

}

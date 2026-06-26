#include "irct/booking_engine.h"

#include <cstdio>

namespace irct {

BookingEngine::BookingEngine()
    : availability_(nullptr), pnr_counter_(1000000000ull), booked_(0) {
    fares_.load_defaults();
    rules_.load_standard();
}

void BookingEngine::set_fare_table(const FareTable &table) {
    fares_ = table;
}

void BookingEngine::set_fare_rules(const FareRules &rules) {
    rules_ = rules;
}

void BookingEngine::attach_availability(Availability *availability) {
    availability_ = availability;
}

std::string BookingEngine::next_pnr() {
    char buf[16];
    uint64_t value = pnr_counter_;
    pnr_counter_ += 7;
    std::snprintf(buf, sizeof(buf), "%010llu",
                  static_cast<unsigned long long>(value % 10000000000ull));
    return std::string(buf);
}

uint32_t BookingEngine::base_fare_for(const BookingRequest &request) const {
    uint32_t base = fares_.total(request.travel_class, request.distance_km);
    uint32_t surcharge = rules_.surcharge(request.distance_km, request.weekday);
    uint32_t multiplier = rules_.peak_multiplier(request.weekday);
    uint32_t adjusted = (base + surcharge) * multiplier / 100u;
    return adjusted;
}

uint32_t BookingEngine::apply_concession(uint32_t base,
                                         ConcessionType type) const {
    if (type == ConcessionType::None) {
        return base;
    }
    return concession_.apply(base, type);
}

bool BookingEngine::seats_available(const BookingRequest &request) const {
    if (!availability_) {
        return true;
    }
    uint16_t needed = static_cast<uint16_t>(request.passengers.size());
    return availability_->open_berths(request.travel_class) >= needed;
}

uint32_t BookingEngine::quote(const BookingRequest &request) const {
    uint32_t base = base_fare_for(request);
    uint32_t per_head = base;
    uint32_t total = 0;
    for (size_t i = 0; i < request.passengers.size(); ++i) {
        ConcessionType type = ConcessionType::None;
        if (i < request.concessions.size()) {
            type = request.concessions[i];
        }
        total += apply_concession(per_head, type);
    }
    return total;
}

BookingResult BookingEngine::book(const BookingRequest &request) {
    BookingResult result;
    result.accepted = false;
    result.waitlisted = false;
    result.fare_paise = 0;
    result.waitlist_position = 0;

    if (request.passengers.empty()) {
        result.message = "no passengers in request";
        return result;
    }

    uint32_t fare = quote(request);
    result.fare_paise = fare;
    result.pnr = next_pnr();

    if (seats_available(request)) {
        if (availability_) {
            availability_->book(request.travel_class,
                                static_cast<uint16_t>(request.passengers.size()));
        }
        result.accepted = true;
        result.message = "confirmed";
        ++booked_;
        return result;
    }

    waitlist_.enqueue(result.pnr,
                      static_cast<uint8_t>(request.passengers.size()));
    result.accepted = true;
    result.waitlisted = true;
    result.waitlist_position = waitlist_.position_of(result.pnr);
    result.message = "waitlisted";
    return result;
}

bool BookingEngine::cancel(const std::string &pnr, uint32_t freed_berths) {
    bool removed = waitlist_.remove(pnr);
    if (freed_berths > 0) {
        waitlist_.promote(freed_berths);
    }
    if (booked_ > 0) {
        --booked_;
    }
    return removed;
}

size_t BookingEngine::total_booked() const {
    return booked_;
}

size_t BookingEngine::total_waitlisted() const {
    return waitlist_.pending();
}

}

#include "irct/chart.h"

#include <algorithm>
#include <cstdio>

namespace irct {

ReservationChart::ReservationChart(uint32_t train_number, uint32_t from_station,
                                   uint32_t to_station)
    : train_number_(train_number),
      from_station_(from_station),
      to_station_(to_station),
      prepared_(false) {
    empty_.pnr = "";
    empty_.passenger_name = "";
    empty_.age = 0;
    empty_.gender = 'U';
    empty_.coach = "";
    empty_.berth = 0;
    empty_.status = BookingStatus::Cancelled;
}

void ReservationChart::add_entry(const ChartEntry &entry) {
    entries_.push_back(entry);
    prepared_ = false;
}

static bool chart_less(const ChartEntry &a, const ChartEntry &b) {
    if (a.coach != b.coach) {
        return a.coach < b.coach;
    }
    return a.berth < b.berth;
}

void ReservationChart::sort_entries() {
    std::sort(entries_.begin(), entries_.end(), chart_less);
}

void ReservationChart::prepare() {
    sort_entries();
    prepared_ = true;
}

bool ReservationChart::is_prepared() const {
    return prepared_;
}

size_t ReservationChart::entry_count() const {
    return entries_.size();
}

size_t ReservationChart::confirmed_count() const {
    size_t total = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].status == BookingStatus::Confirmed) {
            ++total;
        }
    }
    return total;
}

size_t ReservationChart::vacant_after_chart() const {
    size_t vacant = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].status == BookingStatus::Cancelled) {
            ++vacant;
        }
    }
    return vacant;
}

std::vector<ChartEntry> ReservationChart::entries_for_coach(
    const std::string &coach) const {
    std::vector<ChartEntry> out;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].coach == coach) {
            out.push_back(entries_[i]);
        }
    }
    return out;
}

const ChartEntry &ReservationChart::at(size_t i) const {
    if (i >= entries_.size()) {
        return empty_;
    }
    return entries_[i];
}

uint32_t ReservationChart::train_number() const {
    return train_number_;
}

std::string ReservationChart::summary_line() const {
    char buf[96];
    std::snprintf(buf, sizeof(buf),
                  "train %u from %u to %u entries %zu confirmed %zu",
                  train_number_, from_station_, to_station_, entries_.size(),
                  confirmed_count());
    return std::string(buf);
}

void ReservationChart::mark_no_show(const std::string &pnr) {
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].pnr == pnr) {
            entries_[i].status = BookingStatus::Cancelled;
        }
    }
}

}

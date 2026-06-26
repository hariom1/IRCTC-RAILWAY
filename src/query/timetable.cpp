#include "irct/timetable.h"
#include <algorithm>
namespace irct {
Timetable::Timetable() {
    empty_.train_number = 0;
    empty_.station_index = 0;
    empty_.arrival = 0;
    empty_.departure = 0;
    empty_.day_offset = 0;
}
void Timetable::add_entry(const TimetableEntry &e) {
    entries_.push_back(e);
}
std::vector<TimetableEntry> Timetable::entries_for_train(uint32_t train) const {
    std::vector<TimetableEntry> out;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].train_number == train) {
            out.push_back(entries_[i]);
        }
    }
    return out;
}
std::vector<TimetableEntry> Timetable::entries_at_station(uint32_t station) const {
    std::vector<TimetableEntry> out;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].station_index == station) {
            out.push_back(entries_[i]);
        }
    }
    return out;
}
std::vector<TimetableEntry> Timetable::departures_between(uint32_t station, uint16_t lo, uint16_t hi) const {
    std::vector<TimetableEntry> out;
    for (size_t i = 0; i < entries_.size(); ++i) {
        const TimetableEntry &e = entries_[i];
        if (e.station_index != station) {
            continue;
        }
        if (e.departure >= lo && e.departure <= hi) {
            out.push_back(e);
        }
    }
    return out;
}
static bool by_departure(const TimetableEntry &a, const TimetableEntry &b) {
    if (a.day_offset != b.day_offset) {
        return a.day_offset < b.day_offset;
    }
    return a.departure < b.departure;
}
void Timetable::sort_by_departure() {
    std::sort(entries_.begin(), entries_.end(), by_departure);
}
size_t Timetable::size() const { return entries_.size(); }
const TimetableEntry &Timetable::at(size_t i) const {
    if (i >= entries_.size()) {
        return empty_;
    }
    return entries_[i];
}
bool Timetable::busiest_station(uint32_t *out) const {
    if (entries_.empty()) {
        return false;
    }
    std::vector<uint32_t> stations;
    std::vector<uint32_t> counts;
    for (size_t i = 0; i < entries_.size(); ++i) {
        uint32_t s = entries_[i].station_index;
        bool found = false;
        for (size_t j = 0; j < stations.size(); ++j) {
            if (stations[j] == s) {
                counts[j] += 1;
                found = true;
                break;
            }
        }
        if (!found) {
            stations.push_back(s);
            counts.push_back(1);
        }
    }
    size_t best = 0;
    for (size_t j = 1; j < counts.size(); ++j) {
        if (counts[j] > counts[best]) {
            best = j;
        }
    }
    if (out) {
        *out = stations[best];
    }
    return true;
}
}

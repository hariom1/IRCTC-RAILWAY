#ifndef IRCT_TIMETABLE_H
#define IRCT_TIMETABLE_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
struct TimetableEntry {
    uint32_t train_number;
    uint32_t station_index;
    uint16_t arrival;
    uint16_t departure;
    uint8_t day_offset;
};
class Timetable {
public:
    Timetable();
    void add_entry(const TimetableEntry &e);
    std::vector<TimetableEntry> entries_for_train(uint32_t train) const;
    std::vector<TimetableEntry> entries_at_station(uint32_t station) const;
    std::vector<TimetableEntry> departures_between(uint32_t station, uint16_t lo, uint16_t hi) const;
    void sort_by_departure();
    size_t size() const;
    const TimetableEntry &at(size_t i) const;
    bool busiest_station(uint32_t *out) const;
private:
    std::vector<TimetableEntry> entries_;
    TimetableEntry empty_;
};
}
#endif

#ifndef IRCT_CHART_H
#define IRCT_CHART_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/ticket.h"

namespace irct {

struct ChartEntry {
    std::string pnr;
    std::string passenger_name;
    uint8_t age;
    char gender;
    std::string coach;
    uint16_t berth;
    BookingStatus status;
};

class ReservationChart {
public:
    ReservationChart(uint32_t train_number, uint32_t from_station,
                     uint32_t to_station);

    void add_entry(const ChartEntry &entry);

    void prepare();

    bool is_prepared() const;

    size_t entry_count() const;

    size_t confirmed_count() const;

    size_t vacant_after_chart() const;

    std::vector<ChartEntry> entries_for_coach(const std::string &coach) const;

    const ChartEntry &at(size_t i) const;

    uint32_t train_number() const;

    std::string summary_line() const;

    void mark_no_show(const std::string &pnr);

private:
    void sort_entries();

    uint32_t train_number_;
    uint32_t from_station_;
    uint32_t to_station_;
    bool prepared_;
    std::vector<ChartEntry> entries_;
    ChartEntry empty_;
};

}

#endif

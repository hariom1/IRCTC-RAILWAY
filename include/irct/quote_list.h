#ifndef IRCT_QUOTE_LIST_H
#define IRCT_QUOTE_LIST_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/fare_table.h"

namespace irct {

struct ClassQuote {
    FareClass cls;
    uint32_t fare_paise;
    bool available;
};

class QuoteList {
public:
    QuoteList(const FareTable &table, uint32_t distance_km);

    void compute_all();

    void set_availability(FareClass cls, bool available);

    bool cheapest_available(ClassQuote *out) const;

    bool quote_for(FareClass cls, ClassQuote *out) const;

    size_t count() const;

    uint32_t spread() const;

    std::vector<ClassQuote> sorted_by_price() const;

private:
    const FareTable &table_;
    uint32_t distance_km_;
    std::vector<ClassQuote> quotes_;
};

}

#endif

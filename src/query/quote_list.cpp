#include "irct/quote_list.h"

#include <algorithm>

namespace irct {

QuoteList::QuoteList(const FareTable &table, uint32_t distance_km)
    : table_(table), distance_km_(distance_km) {}

void QuoteList::compute_all() {
    quotes_.clear();
    FareClass classes[5] = {FareClass::Second, FareClass::Sleeper,
                            FareClass::ThirdAc, FareClass::SecondAc,
                            FareClass::FirstAc};
    for (int i = 0; i < 5; ++i) {
        ClassQuote q;
        q.cls = classes[i];
        q.fare_paise = table_.total(classes[i], distance_km_);
        q.available = true;
        quotes_.push_back(q);
    }
}

void QuoteList::set_availability(FareClass cls, bool available) {
    for (size_t i = 0; i < quotes_.size(); ++i) {
        if (quotes_[i].cls == cls) {
            quotes_[i].available = available;
            return;
        }
    }
}

bool QuoteList::cheapest_available(ClassQuote *out) const {
    const ClassQuote *best = nullptr;
    for (size_t i = 0; i < quotes_.size(); ++i) {
        if (!quotes_[i].available) {
            continue;
        }
        if (!best || quotes_[i].fare_paise < best->fare_paise) {
            best = &quotes_[i];
        }
    }
    if (!best) {
        return false;
    }
    if (out) {
        *out = *best;
    }
    return true;
}

bool QuoteList::quote_for(FareClass cls, ClassQuote *out) const {
    for (size_t i = 0; i < quotes_.size(); ++i) {
        if (quotes_[i].cls == cls) {
            if (out) {
                *out = quotes_[i];
            }
            return true;
        }
    }
    return false;
}

size_t QuoteList::count() const {
    return quotes_.size();
}

uint32_t QuoteList::spread() const {
    if (quotes_.empty()) {
        return 0;
    }
    uint32_t lo = quotes_[0].fare_paise;
    uint32_t hi = quotes_[0].fare_paise;
    for (size_t i = 1; i < quotes_.size(); ++i) {
        if (quotes_[i].fare_paise < lo) {
            lo = quotes_[i].fare_paise;
        }
        if (quotes_[i].fare_paise > hi) {
            hi = quotes_[i].fare_paise;
        }
    }
    return hi - lo;
}

static bool quote_cheaper(const ClassQuote &a, const ClassQuote &b) {
    return a.fare_paise < b.fare_paise;
}

std::vector<ClassQuote> QuoteList::sorted_by_price() const {
    std::vector<ClassQuote> out = quotes_;
    std::sort(out.begin(), out.end(), quote_cheaper);
    return out;
}

}

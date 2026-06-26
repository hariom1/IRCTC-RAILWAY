#include "irct/station_index.h"

#include <algorithm>

namespace irct {

StationIndex::StationIndex() {
    empty_.index = 0;
    empty_.code = "";
    empty_.zone = 0;
}

std::string StationIndex::upper(const std::string &s) {
    std::string out = s;
    for (size_t i = 0; i < out.size(); ++i) {
        char c = out[i];
        if (c >= 'a' && c <= 'z') {
            out[i] = static_cast<char>(c - 'a' + 'A');
        }
    }
    return out;
}

bool StationIndex::has_prefix(const std::string &value,
                              const std::string &prefix) {
    if (prefix.size() > value.size()) {
        return false;
    }
    for (size_t i = 0; i < prefix.size(); ++i) {
        if (value[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

static bool record_less(const StationRecord &a, const StationRecord &b) {
    return a.code < b.code;
}

void StationIndex::build(const irct_catalogue *cat) {
    records_.clear();
    size_t n = irct_station_count(cat);
    for (size_t i = 0; i < n; ++i) {
        StationRecord rec;
        rec.index = static_cast<uint32_t>(i);
        const char *name = irct_station_name(cat, i);
        rec.code = upper(name ? name : "");
        uint16_t zone = 0;
        irct_station_zone(cat, i, &zone);
        rec.zone = zone;
        records_.push_back(rec);
    }
    std::sort(records_.begin(), records_.end(), record_less);
}

bool StationIndex::lookup_by_code(const std::string &code,
                                  StationRecord *out) const {
    std::string key = upper(code);
    size_t lo = 0;
    size_t hi = records_.size();
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (records_[mid].code < key) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo < records_.size() && records_[lo].code == key) {
        if (out) {
            *out = records_[lo];
        }
        return true;
    }
    return false;
}

std::vector<StationRecord> StationIndex::prefix_search(
    const std::string &prefix) const {
    std::string key = upper(prefix);
    std::vector<StationRecord> result;
    for (size_t i = 0; i < records_.size(); ++i) {
        if (has_prefix(records_[i].code, key)) {
            result.push_back(records_[i]);
        }
    }
    return result;
}

std::vector<StationRecord> StationIndex::by_zone(uint16_t zone) const {
    std::vector<StationRecord> result;
    for (size_t i = 0; i < records_.size(); ++i) {
        if (records_[i].zone == zone) {
            result.push_back(records_[i]);
        }
    }
    return result;
}

size_t StationIndex::size() const {
    return records_.size();
}

const StationRecord &StationIndex::at(size_t i) const {
    if (i >= records_.size()) {
        return empty_;
    }
    return records_[i];
}

void StationIndex::clear() {
    records_.clear();
}

}

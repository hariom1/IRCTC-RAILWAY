#ifndef IRCT_STATION_INDEX_H
#define IRCT_STATION_INDEX_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct.h"

namespace irct {

struct StationRecord {
    uint32_t index;
    std::string code;
    uint16_t zone;
};

class StationIndex {
public:
    StationIndex();

    void build(const irct_catalogue *cat);

    bool lookup_by_code(const std::string &code, StationRecord *out) const;

    std::vector<StationRecord> prefix_search(const std::string &prefix) const;

    std::vector<StationRecord> by_zone(uint16_t zone) const;

    size_t size() const;

    const StationRecord &at(size_t i) const;

    void clear();

private:
    static std::string upper(const std::string &s);
    static bool has_prefix(const std::string &value, const std::string &prefix);

    std::vector<StationRecord> records_;
    StationRecord empty_;
};

}

#endif

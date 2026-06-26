#ifndef IRCT_CATALOGUE_QUERY_H
#define IRCT_CATALOGUE_QUERY_H

#include <string>
#include <vector>

#include "irct.h"

namespace irct {

struct StationInfo {
    size_t index;
    std::string name;
    uint16_t zone;
};

class CatalogueQuery {
public:
    explicit CatalogueQuery(const irct_catalogue *cat);

    size_t station_count() const;

    size_t train_count() const;

    bool find_station_by_name(const std::string &name, StationInfo *out) const;

    std::vector<StationInfo> stations_in_zone(uint16_t zone) const;

    std::vector<std::string> all_station_names() const;

    bool valid() const;

private:
    const irct_catalogue *cat_;
};

}

#endif

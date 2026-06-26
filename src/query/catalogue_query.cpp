#include "irct/catalogue_query.h"

namespace irct {

CatalogueQuery::CatalogueQuery(const irct_catalogue *cat) : cat_(cat) {}

bool CatalogueQuery::valid() const {
    return cat_ != nullptr;
}

size_t CatalogueQuery::station_count() const {
    return irct_station_count(cat_);
}

size_t CatalogueQuery::train_count() const {
    return irct_train_count(cat_);
}

bool CatalogueQuery::find_station_by_name(const std::string &name,
                                          StationInfo *out) const {
    size_t n = station_count();
    for (size_t i = 0; i < n; ++i) {
        const char *cur = irct_station_name(cat_, i);
        if (cur && name == cur) {
            if (out) {
                out->index = i;
                out->name = cur;
                uint16_t zone = 0;
                irct_station_zone(cat_, i, &zone);
                out->zone = zone;
            }
            return true;
        }
    }
    return false;
}

std::vector<StationInfo> CatalogueQuery::stations_in_zone(uint16_t zone) const {
    std::vector<StationInfo> result;
    size_t n = station_count();
    for (size_t i = 0; i < n; ++i) {
        uint16_t z = 0;
        if (irct_station_zone(cat_, i, &z) != 0) {
            continue;
        }
        if (z != zone) {
            continue;
        }
        StationInfo info;
        info.index = i;
        const char *name = irct_station_name(cat_, i);
        info.name = name ? name : "";
        info.zone = z;
        result.push_back(info);
    }
    return result;
}

std::vector<std::string> CatalogueQuery::all_station_names() const {
    std::vector<std::string> names;
    size_t n = station_count();
    for (size_t i = 0; i < n; ++i) {
        const char *name = irct_station_name(cat_, i);
        names.push_back(name ? name : "");
    }
    return names;
}

}

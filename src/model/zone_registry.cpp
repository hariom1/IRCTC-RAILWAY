#include "irct/zone_registry.h"

namespace irct {

ZoneRegistry::ZoneRegistry() {}

void ZoneRegistry::load_defaults() {
    zones_.clear();
    divisions_.clear();

    struct Seed {
        uint16_t id;
        const char *code;
        const char *name;
        const char *hq;
    };

    static const Seed seeds[] = {
        {1, "NR", "Northern Railway", "New Delhi"},
        {2, "WR", "Western Railway", "Mumbai"},
        {3, "CR", "Central Railway", "Mumbai"},
        {4, "ER", "Eastern Railway", "Kolkata"},
        {5, "SR", "Southern Railway", "Chennai"},
        {6, "SCR", "South Central Railway", "Secunderabad"},
        {7, "SER", "South Eastern Railway", "Kolkata"},
        {8, "NER", "North Eastern Railway", "Gorakhpur"},
        {9, "NFR", "Northeast Frontier Railway", "Guwahati"},
        {10, "SWR", "South Western Railway", "Hubballi"},
        {11, "NWR", "North Western Railway", "Jaipur"},
        {12, "WCR", "West Central Railway", "Jabalpur"},
        {13, "NCR", "North Central Railway", "Prayagraj"},
        {14, "SECR", "South East Central Railway", "Bilaspur"},
        {15, "ECR", "East Central Railway", "Hajipur"},
        {16, "ECoR", "East Coast Railway", "Bhubaneswar"},
        {17, "WCtR", "West Coast Railway", "Madgaon"},
    };

    size_t count = sizeof(seeds) / sizeof(seeds[0]);
    for (size_t i = 0; i < count; ++i) {
        Zone zone;
        zone.id = seeds[i].id;
        zone.code = seeds[i].code;
        zone.name = seeds[i].name;
        zone.headquarters = seeds[i].hq;
        zones_.push_back(zone);
    }
}

bool ZoneRegistry::zone_by_id(uint16_t id, Zone *out) const {
    for (size_t i = 0; i < zones_.size(); ++i) {
        if (zones_[i].id == id) {
            if (out) {
                *out = zones_[i];
            }
            return true;
        }
    }
    return false;
}

bool ZoneRegistry::zone_by_code(const std::string &code, Zone *out) const {
    for (size_t i = 0; i < zones_.size(); ++i) {
        if (zones_[i].code == code) {
            if (out) {
                *out = zones_[i];
            }
            return true;
        }
    }
    return false;
}

void ZoneRegistry::add_division(const std::string &code,
                                const std::string &name, uint16_t zone_id) {
    Division div;
    div.code = code;
    div.name = name;
    div.zone_id = zone_id;
    divisions_.push_back(div);
}

std::vector<Division> ZoneRegistry::divisions_in_zone(uint16_t zone_id) const {
    std::vector<Division> out;
    for (size_t i = 0; i < divisions_.size(); ++i) {
        if (divisions_[i].zone_id == zone_id) {
            out.push_back(divisions_[i]);
        }
    }
    return out;
}

size_t ZoneRegistry::zone_count() const {
    return zones_.size();
}

size_t ZoneRegistry::division_count() const {
    return divisions_.size();
}

const char *ZoneRegistry::zone_name_for(uint16_t id) const {
    for (size_t i = 0; i < zones_.size(); ++i) {
        if (zones_[i].id == id) {
            return zones_[i].name.c_str();
        }
    }
    return "unknown";
}

}

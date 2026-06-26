#ifndef IRCT_ZONE_REGISTRY_H
#define IRCT_ZONE_REGISTRY_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

struct Division {
    std::string code;
    std::string name;
    uint16_t zone_id;
};

struct Zone {
    uint16_t id;
    std::string code;
    std::string name;
    std::string headquarters;
};

class ZoneRegistry {
public:
    ZoneRegistry();

    void load_defaults();

    bool zone_by_id(uint16_t id, Zone *out) const;

    bool zone_by_code(const std::string &code, Zone *out) const;

    void add_division(const std::string &code, const std::string &name,
                      uint16_t zone_id);

    std::vector<Division> divisions_in_zone(uint16_t zone_id) const;

    size_t zone_count() const;

    size_t division_count() const;

    const char *zone_name_for(uint16_t id) const;

private:
    std::vector<Zone> zones_;
    std::vector<Division> divisions_;
};

}

#endif

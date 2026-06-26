#ifndef IRCT_FARE_TABLE_H
#define IRCT_FARE_TABLE_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace irct {

enum class FareClass {
    Second,
    Sleeper,
    ThirdAc,
    SecondAc,
    FirstAc
};

struct FareSlab {
    uint32_t up_to_km;
    uint32_t base_paise;
    uint32_t per_km_paise;
};

class FareTable {
public:
    FareTable();

    void add_slab(FareClass cls, const FareSlab &slab);

    uint32_t quote(FareClass cls, uint32_t distance_km) const;

    uint32_t reservation_fee(FareClass cls) const;

    uint32_t total(FareClass cls, uint32_t distance_km) const;

    size_t slab_count(FareClass cls) const;

    void load_defaults();

private:
    static size_t class_index(FareClass cls);
    std::vector<FareSlab> slabs_[5];
};

}

#endif

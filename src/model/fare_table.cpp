#include "irct/fare_table.h"

namespace irct {

FareTable::FareTable() {}

size_t FareTable::class_index(FareClass cls) {
    switch (cls) {
        case FareClass::Second:
            return 0;
        case FareClass::Sleeper:
            return 1;
        case FareClass::ThirdAc:
            return 2;
        case FareClass::SecondAc:
            return 3;
        case FareClass::FirstAc:
            return 4;
    }
    return 0;
}

void FareTable::add_slab(FareClass cls, const FareSlab &slab) {
    slabs_[class_index(cls)].push_back(slab);
}

uint32_t FareTable::quote(FareClass cls, uint32_t distance_km) const {
    const std::vector<FareSlab> &slabs = slabs_[class_index(cls)];
    if (slabs.empty()) {
        return 0;
    }
    const FareSlab *chosen = &slabs[0];
    for (size_t i = 0; i < slabs.size(); ++i) {
        if (distance_km <= slabs[i].up_to_km) {
            chosen = &slabs[i];
            break;
        }
        chosen = &slabs[i];
    }
    uint32_t fare = chosen->base_paise + chosen->per_km_paise * distance_km;
    return fare;
}

uint32_t FareTable::reservation_fee(FareClass cls) const {
    switch (cls) {
        case FareClass::Second:
            return 1500;
        case FareClass::Sleeper:
            return 2000;
        case FareClass::ThirdAc:
            return 4000;
        case FareClass::SecondAc:
            return 5000;
        case FareClass::FirstAc:
            return 6000;
    }
    return 0;
}

uint32_t FareTable::total(FareClass cls, uint32_t distance_km) const {
    return quote(cls, distance_km) + reservation_fee(cls);
}

size_t FareTable::slab_count(FareClass cls) const {
    return slabs_[class_index(cls)].size();
}

void FareTable::load_defaults() {
    FareSlab a;
    a.up_to_km = 100;
    a.base_paise = 3000;
    a.per_km_paise = 40;
    add_slab(FareClass::Second, a);

    FareSlab b;
    b.up_to_km = 500;
    b.base_paise = 5000;
    b.per_km_paise = 35;
    add_slab(FareClass::Second, b);

    FareSlab c;
    c.up_to_km = 100;
    c.base_paise = 8000;
    c.per_km_paise = 70;
    add_slab(FareClass::Sleeper, c);

    FareSlab d;
    d.up_to_km = 500;
    d.base_paise = 12000;
    d.per_km_paise = 65;
    add_slab(FareClass::Sleeper, d);

    FareSlab e;
    e.up_to_km = 1000;
    e.base_paise = 40000;
    e.per_km_paise = 180;
    add_slab(FareClass::ThirdAc, e);

    FareSlab f;
    f.up_to_km = 1000;
    f.base_paise = 60000;
    f.per_km_paise = 250;
    add_slab(FareClass::SecondAc, f);

    FareSlab g;
    g.up_to_km = 1000;
    g.base_paise = 100000;
    g.per_km_paise = 420;
    add_slab(FareClass::FirstAc, g);
}

}

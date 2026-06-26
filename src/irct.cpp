#include "irct.h"
#include "reader.h"
#include "irct/format.h"

#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>

namespace {

const uint32_t kMagic = irct::format::kMagic;

const uint16_t kSupportedVersion = irct::format::kVersion;

const uint8_t kSectionStrings = irct::format::kSectionStrings;
const uint8_t kSectionStations = irct::format::kSectionStations;
const uint8_t kSectionTrains = irct::format::kSectionTrains;
const uint8_t kSectionEnd = irct::format::kSectionEnd;

const uint32_t kMaxStringBlob = irct::format::kMaxStringBlob;
const uint32_t kMaxStations = irct::format::kMaxStations;
const uint32_t kMaxTrains = irct::format::kMaxTrains;
const uint32_t kMaxStopsDecl = irct::format::kMaxStopsDecl;

struct Stop {
    uint32_t station_index;
    uint16_t arrival;
    uint16_t departure;
    uint8_t halt_kind;
};

struct Train {
    uint32_t number;
    Stop *stops;
    uint32_t stop_count;
    uint32_t stop_cap;
};

struct StringTable {
    char *blob;
    uint32_t blob_len;
    uint32_t *offsets;
    uint32_t count;
};

struct Station {
    uint32_t name_ref;
    uint16_t zone;
};

}

struct irct_catalogue {
    StringTable strings;
    Station *stations;
    uint32_t station_count;
    Train *trains;
    uint32_t train_count;
};

namespace {

void train_reserve(Train *t, uint32_t need) {
    if (need <= t->stop_cap) {
        return;
    }
    uint32_t cap = t->stop_cap ? t->stop_cap : 4;
    while (cap < need) {
        cap *= 2;
    }
    Stop *grown = static_cast<Stop *>(std::realloc(t->stops, cap * sizeof(Stop)));
    if (!grown) {
        return;
    }
    t->stops = grown;
    t->stop_cap = cap;
}

bool parse_strings(irct::Reader &r, StringTable *st) {
    uint32_t count = r.u32();
    uint32_t blob_len = r.u32();
    if (!r.ok()) {
        return false;
    }
    if (count == 0 || count > kMaxStations || blob_len == 0 ||
        blob_len > kMaxStringBlob) {
        return false;
    }

    st->offsets = static_cast<uint32_t *>(std::malloc(count * sizeof(uint32_t)));
    if (!st->offsets) {
        return false;
    }
    st->count = count;

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t off = r.u32();
        if (!r.ok() || off >= blob_len) {
            return false;
        }
        st->offsets[i] = off;
    }

    const uint8_t *raw = r.take(blob_len);
    if (!raw) {
        return false;
    }
    st->blob = static_cast<char *>(std::malloc(blob_len + 1));
    if (!st->blob) {
        return false;
    }
    std::memcpy(st->blob, raw, blob_len);
    st->blob[blob_len] = '\0';
    st->blob_len = blob_len;
    return true;
}

bool parse_stations(irct::Reader &r, irct_catalogue *cat) {
    uint32_t count = r.u32();
    if (!r.ok()) {
        return false;
    }
    if (count == 0 || count > kMaxStations) {
        return false;
    }
    cat->stations = static_cast<Station *>(std::malloc(count * sizeof(Station)));
    if (!cat->stations) {
        return false;
    }
    cat->station_count = count;
    for (uint32_t i = 0; i < count; ++i) {
        uint32_t ref = r.u32();
        uint16_t zone = r.u16();
        if (!r.ok()) {
            return false;
        }
        if (ref >= cat->strings.count) {
            return false;
        }
        cat->stations[i].name_ref = ref;
        cat->stations[i].zone = zone;
    }
    return true;
}

bool read_stop(irct::Reader &r, irct_catalogue *cat, Train *t, uint32_t slot) {
    uint32_t station_index = r.u32();
    uint16_t arrival = r.u16();
    uint16_t departure = r.u16();
    uint8_t halt_kind = r.u8();
    uint8_t ext_count = r.u8();
    if (!r.ok()) {
        return false;
    }
    if (station_index >= cat->station_count) {
        return false;
    }

    Stop *write = &t->stops[slot];

    if (ext_count > 0) {
        uint32_t base = t->stop_count;
        train_reserve(t, base + ext_count);
        for (uint32_t e = 0; e < ext_count; ++e) {
            uint32_t ext_station = r.u32();
            uint8_t ext_kind = r.u8();
            if (!r.ok()) {
                return false;
            }
            if (ext_station >= cat->station_count) {
                return false;
            }
            Stop *es = &t->stops[base + e];
            es->station_index = ext_station;
            es->arrival = arrival;
            es->departure = departure;
            es->halt_kind = ext_kind;
        }
        t->stop_count = base + ext_count;
    }

    write->station_index = station_index;
    write->arrival = arrival;
    write->departure = departure;
    write->halt_kind = halt_kind;
    return true;
}

bool parse_train(irct::Reader &r, irct_catalogue *cat, Train *t) {
    t->number = r.u32();
    uint32_t declared = r.u32();
    if (!r.ok()) {
        return false;
    }
    if (declared == 0 || declared > kMaxStopsDecl) {
        return false;
    }

    t->stops = static_cast<Stop *>(std::malloc(declared * sizeof(Stop)));
    if (!t->stops) {
        return false;
    }
    t->stop_cap = declared;
    t->stop_count = 0;

    for (uint32_t s = 0; s < declared; ++s) {
        uint32_t slot = t->stop_count;
        if (!read_stop(r, cat, t, slot)) {
            return false;
        }
        if (t->stop_count <= slot) {
            t->stop_count = slot + 1;
        }
    }
    return true;
}

bool parse_trains(irct::Reader &r, irct_catalogue *cat) {
    uint32_t count = r.u32();
    if (!r.ok()) {
        return false;
    }
    if (count == 0 || count > kMaxTrains) {
        return false;
    }
    cat->trains = static_cast<Train *>(std::calloc(count, sizeof(Train)));
    if (!cat->trains) {
        return false;
    }
    cat->train_count = count;
    for (uint32_t i = 0; i < count; ++i) {
        if (!parse_train(r, cat, &cat->trains[i])) {
            return false;
        }
    }
    return true;
}

void free_internal(irct_catalogue *cat) {
    if (!cat) {
        return;
    }
    std::free(cat->strings.offsets);
    std::free(cat->strings.blob);
    std::free(cat->stations);
    if (cat->trains) {
        for (uint32_t i = 0; i < cat->train_count; ++i) {
            std::free(cat->trains[i].stops);
        }
        std::free(cat->trains);
    }
    std::free(cat);
}

}

extern "C" irct_catalogue *irct_parse(const uint8_t *data, size_t size,
                                      irct_status *status) {
    irct_status local;
    if (!status) {
        status = &local;
    }
    *status = IRCT_OK;

    if (!data || size < 8) {
        *status = IRCT_ERR_SHORT;
        return nullptr;
    }

    irct::Reader r(data, size);
    uint32_t magic = r.u32();
    if (magic != kMagic) {
        *status = IRCT_ERR_MAGIC;
        return nullptr;
    }
    uint16_t version = r.u16();
    uint16_t section_count = r.u16();
    if (version != kSupportedVersion) {
        *status = IRCT_ERR_VERSION;
        return nullptr;
    }

    irct_catalogue *cat =
        static_cast<irct_catalogue *>(std::calloc(1, sizeof(irct_catalogue)));
    if (!cat) {
        *status = IRCT_ERR_ALLOC;
        return nullptr;
    }

    bool have_strings = false;
    bool have_stations = false;

    for (uint16_t s = 0; s < section_count; ++s) {
        uint8_t tag = r.u8();
        if (!r.ok()) {
            *status = IRCT_ERR_SECTION;
            free_internal(cat);
            return nullptr;
        }
        if (tag == kSectionEnd) {
            break;
        }
        if (tag == kSectionStrings) {
            if (have_strings || !parse_strings(r, &cat->strings)) {
                *status = IRCT_ERR_FORMAT;
                free_internal(cat);
                return nullptr;
            }
            have_strings = true;
        } else if (tag == kSectionStations) {
            if (!have_strings || have_stations ||
                !parse_stations(r, cat)) {
                *status = IRCT_ERR_FORMAT;
                free_internal(cat);
                return nullptr;
            }
            have_stations = true;
        } else if (tag == kSectionTrains) {
            if (!have_stations || !parse_trains(r, cat)) {
                *status = IRCT_ERR_FORMAT;
                free_internal(cat);
                return nullptr;
            }
        } else {
            *status = IRCT_ERR_SECTION;
            free_internal(cat);
            return nullptr;
        }
    }

    return cat;
}

extern "C" void irct_free(irct_catalogue *cat) {
    free_internal(cat);
}

extern "C" size_t irct_station_count(const irct_catalogue *cat) {
    return cat ? cat->station_count : 0;
}

extern "C" size_t irct_train_count(const irct_catalogue *cat) {
    return cat ? cat->train_count : 0;
}

extern "C" const char *irct_station_name(const irct_catalogue *cat,
                                         size_t index) {
    if (!cat || index >= cat->station_count) {
        return nullptr;
    }
    uint32_t ref = cat->stations[index].name_ref;
    if (ref >= cat->strings.count) {
        return nullptr;
    }
    uint32_t off = cat->strings.offsets[ref];
    if (off >= cat->strings.blob_len) {
        return nullptr;
    }
    return cat->strings.blob + off;
}

extern "C" int irct_station_zone(const irct_catalogue *cat, size_t index,
                                 uint16_t *zone) {
    if (!cat || !zone || index >= cat->station_count) {
        return -1;
    }
    *zone = cat->stations[index].zone;
    return 0;
}

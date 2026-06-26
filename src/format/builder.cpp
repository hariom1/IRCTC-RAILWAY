#include "irct/builder.h"

#include "irct/format.h"

namespace irct {

CatalogueBuilder::CatalogueBuilder() {}

uint32_t CatalogueBuilder::add_name(const std::string &name) {
    uint32_t id = static_cast<uint32_t>(names_.size());
    names_.push_back(name);
    return id;
}

uint32_t CatalogueBuilder::add_station(uint32_t name_ref, uint16_t zone) {
    BuildStation s;
    s.name_ref = name_ref;
    s.zone = zone;
    uint32_t id = static_cast<uint32_t>(stations_.size());
    stations_.push_back(s);
    return id;
}

void CatalogueBuilder::add_train(const BuildTrain &train) {
    trains_.push_back(train);
}

std::vector<uint8_t> CatalogueBuilder::build() {
    ByteBuffer buf;
    buf.put_u32(format::kMagic);
    buf.put_u16(format::kVersion);
    buf.put_u16(4);

    buf.put_u8(format::kSectionStrings);
    ByteBuffer blob;
    std::vector<uint32_t> offsets;
    for (size_t i = 0; i < names_.size(); ++i) {
        offsets.push_back(static_cast<uint32_t>(blob.size()));
        blob.put_cstr(names_[i].c_str());
    }
    buf.put_u32(static_cast<uint32_t>(offsets.size()));
    buf.put_u32(static_cast<uint32_t>(blob.size()));
    for (size_t i = 0; i < offsets.size(); ++i) {
        buf.put_u32(offsets[i]);
    }
    buf.put_bytes(blob.data(), blob.size());

    buf.put_u8(format::kSectionStations);
    buf.put_u32(static_cast<uint32_t>(stations_.size()));
    for (size_t i = 0; i < stations_.size(); ++i) {
        buf.put_u32(stations_[i].name_ref);
        buf.put_u16(stations_[i].zone);
    }

    buf.put_u8(format::kSectionTrains);
    buf.put_u32(static_cast<uint32_t>(trains_.size()));
    for (size_t t = 0; t < trains_.size(); ++t) {
        const BuildTrain &tr = trains_[t];
        buf.put_u32(tr.number);
        buf.put_u32(static_cast<uint32_t>(tr.stops.size()));
        for (size_t s = 0; s < tr.stops.size(); ++s) {
            const BuildStop &st = tr.stops[s];
            buf.put_u32(st.station_index);
            buf.put_u16(st.arrival);
            buf.put_u16(st.departure);
            buf.put_u8(st.halt_kind);
            buf.put_u8(static_cast<uint8_t>(st.extensions.size()));
            for (size_t e = 0; e < st.extensions.size(); ++e) {
                buf.put_u32(st.extensions[e].first);
                buf.put_u8(st.extensions[e].second);
            }
        }
    }

    buf.put_u8(format::kSectionEnd);
    return buf.take();
}

}

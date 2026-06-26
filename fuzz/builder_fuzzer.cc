#include <cstddef>
#include <cstdint>

#include "irct.h"
#include "irct/builder.h"
#include "irct/catalogue_query.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    irct::CatalogueBuilder builder;

    size_t name_count = (size % 5) + 1;
    for (size_t i = 0; i < name_count; ++i) {
        char buf[8];
        buf[0] = 'S';
        buf[1] = static_cast<char>('A' + (i % 26));
        buf[2] = '\0';
        builder.add_name(buf);
    }
    for (size_t i = 0; i < name_count; ++i) {
        builder.add_station(static_cast<uint32_t>(i),
                            static_cast<uint16_t>(i + 1));
    }

    irct::BuildTrain train;
    train.number = static_cast<uint32_t>(size + 1);
    irct::BuildStop stop;
    stop.station_index = 0;
    stop.arrival = static_cast<uint16_t>(size & 0xFFFF);
    stop.departure = static_cast<uint16_t>((size + 1) & 0xFFFF);
    stop.halt_kind = static_cast<uint8_t>(size & 0xFF);
    train.stops.push_back(stop);
    builder.add_train(train);

    std::vector<uint8_t> bytes = builder.build();

    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(bytes.data(), bytes.size(), &status);
    if (cat) {
        irct::CatalogueQuery query(cat);
        (void)query.station_count();
        (void)query.all_station_names();
        irct_free(cat);
    }
    return 0;
}

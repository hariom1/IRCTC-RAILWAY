#ifndef IRCT_BUILDER_H
#define IRCT_BUILDER_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/bytebuffer.h"

namespace irct {

struct BuildStop {
    uint32_t station_index;
    uint16_t arrival;
    uint16_t departure;
    uint8_t halt_kind;
    std::vector<std::pair<uint32_t, uint8_t> > extensions;
};

struct BuildTrain {
    uint32_t number;
    std::vector<BuildStop> stops;
};

struct BuildStation {
    uint32_t name_ref;
    uint16_t zone;
};

class CatalogueBuilder {
public:
    CatalogueBuilder();

    uint32_t add_name(const std::string &name);

    uint32_t add_station(uint32_t name_ref, uint16_t zone);

    void add_train(const BuildTrain &train);

    std::vector<uint8_t> build();

private:
    std::vector<std::string> names_;
    std::vector<BuildStation> stations_;
    std::vector<BuildTrain> trains_;
};

}

#endif

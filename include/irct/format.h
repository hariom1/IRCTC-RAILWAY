#ifndef IRCT_FORMAT_H
#define IRCT_FORMAT_H

#include <cstdint>

namespace irct {
namespace format {

const uint32_t kMagic = 0x54435249u;
const uint16_t kVersion = 3;

const uint8_t kSectionStrings = 0x10;
const uint8_t kSectionStations = 0x20;
const uint8_t kSectionTrains = 0x30;
const uint8_t kSectionEnd = 0xFF;

const uint8_t kHaltTechnical = 0;
const uint8_t kHaltCommercial = 1;
const uint8_t kHaltTerminal = 2;

const uint32_t kMaxStringBlob = 1u << 20;
const uint32_t kMaxStations = 1u << 16;
const uint32_t kMaxTrains = 1u << 16;
const uint32_t kMaxStopsDecl = 1u << 12;

}
}

#endif

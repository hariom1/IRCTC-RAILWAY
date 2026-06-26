#include <cstdio>
#include <cstdlib>
#include <vector>

#include "irct.h"
#include "irct/catalogue_query.h"
#include "irct/crc32.h"
#include "irct/diagnostics.h"

static int load_file(const char *path, std::vector<unsigned char> &out) {
    FILE *f = std::fopen(path, "rb");
    if (!f) {
        return -1;
    }
    std::fseek(f, 0, SEEK_END);
    long n = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (n < 0) {
        std::fclose(f);
        return -1;
    }
    out.resize(static_cast<size_t>(n));
    if (n > 0 && std::fread(out.data(), 1, static_cast<size_t>(n), f) !=
                     static_cast<size_t>(n)) {
        std::fclose(f);
        return -1;
    }
    std::fclose(f);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: %s <catalogue.irct>\n", argv[0]);
        return 2;
    }

    std::vector<unsigned char> data;
    if (load_file(argv[1], data) != 0) {
        std::fprintf(stderr, "cannot read %s\n", argv[1]);
        return 2;
    }

    uint32_t crc = irct::Crc32::compute(data.data(), data.size());
    std::printf("input crc32: %08x\n", crc);

    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(data.data(), data.size(), &status);
    if (!cat) {
        std::fprintf(stderr, "parse failed, status %s\n",
                     irct::DiagnosticSink::status_name(status));
        return 1;
    }

    irct::CatalogueQuery query(cat);
    std::printf("stations: %zu\n", query.station_count());
    std::printf("trains: %zu\n", query.train_count());

    std::vector<std::string> names = query.all_station_names();
    for (size_t i = 0; i < names.size(); ++i) {
        uint16_t zone = 0;
        irct_station_zone(cat, i, &zone);
        std::printf("  station %zu: %s (zone %u)\n", i, names[i].c_str(),
                    static_cast<unsigned>(zone));
    }

    irct_free(cat);
    return 0;
}

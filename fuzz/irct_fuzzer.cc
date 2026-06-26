#include <cstddef>
#include <cstdint>

#include "irct.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(data, size, &status);
    if (cat) {
        size_t stations = irct_station_count(cat);
        for (size_t i = 0; i < stations; ++i) {
            const char *name = irct_station_name(cat, i);
            if (name) {
                volatile char c = name[0];
                (void)c;
            }
        }
        (void)irct_train_count(cat);
        irct_free(cat);
    }
    return 0;
}

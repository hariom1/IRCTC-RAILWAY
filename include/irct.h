#ifndef IRCT_H
#define IRCT_H

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IRCT_OK = 0,
    IRCT_ERR_SHORT = 1,
    IRCT_ERR_MAGIC = 2,
    IRCT_ERR_VERSION = 3,
    IRCT_ERR_SECTION = 4,
    IRCT_ERR_RANGE = 5,
    IRCT_ERR_ALLOC = 6,
    IRCT_ERR_FORMAT = 7
} irct_status;

typedef struct irct_catalogue irct_catalogue;

irct_catalogue *irct_parse(const uint8_t *data, size_t size, irct_status *status);

void irct_free(irct_catalogue *cat);

size_t irct_station_count(const irct_catalogue *cat);

size_t irct_train_count(const irct_catalogue *cat);

const char *irct_station_name(const irct_catalogue *cat, size_t index);

int irct_station_zone(const irct_catalogue *cat, size_t index, uint16_t *zone);

#ifdef __cplusplus
}
#endif

#endif

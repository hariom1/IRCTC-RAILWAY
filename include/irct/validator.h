#ifndef IRCT_VALIDATOR_H
#define IRCT_VALIDATOR_H

#include "irct.h"
#include "irct/diagnostics.h"

namespace irct {

class CatalogueValidator {
public:
    CatalogueValidator();

    bool validate(const irct_catalogue *cat, DiagnosticSink *sink);

    size_t checked_stations() const;

    size_t checked_trains() const;

private:
    bool check_station_names(const irct_catalogue *cat, DiagnosticSink *sink);

    bool check_train_count(const irct_catalogue *cat, DiagnosticSink *sink);

    size_t stations_;
    size_t trains_;
};

}

#endif

#include "irct/validator.h"

#include "irct/catalogue_query.h"
#include "irct/schedule.h"

namespace irct {

CatalogueValidator::CatalogueValidator() : stations_(0), trains_(0) {}

bool CatalogueValidator::check_station_names(const irct_catalogue *cat,
                                             DiagnosticSink *sink) {
    bool ok = true;
    size_t n = irct_station_count(cat);
    for (size_t i = 0; i < n; ++i) {
        const char *name = irct_station_name(cat, i);
        if (!name || name[0] == '\0') {
            if (sink) {
                sink->warn(i, "station has an empty name");
            }
            ok = false;
        }
        ++stations_;
    }
    return ok;
}

bool CatalogueValidator::check_train_count(const irct_catalogue *cat,
                                           DiagnosticSink *sink) {
    size_t n = irct_train_count(cat);
    trains_ = n;
    if (n == 0) {
        if (sink) {
            sink->info(0, "catalogue declares no trains");
        }
    }
    return true;
}

bool CatalogueValidator::validate(const irct_catalogue *cat,
                                  DiagnosticSink *sink) {
    if (!cat) {
        if (sink) {
            sink->error(0, "null catalogue");
        }
        return false;
    }
    stations_ = 0;
    trains_ = 0;
    bool names_ok = check_station_names(cat, sink);
    bool trains_ok = check_train_count(cat, sink);

    CatalogueQuery query(cat);
    std::vector<std::string> names = query.all_station_names();
    for (size_t i = 0; i < names.size(); ++i) {
        for (size_t j = i + 1; j < names.size(); ++j) {
            if (!names[i].empty() && names[i] == names[j]) {
                if (sink) {
                    sink->warn(j, "duplicate station name");
                }
            }
        }
    }

    return names_ok && trains_ok;
}

size_t CatalogueValidator::checked_stations() const {
    return stations_;
}

size_t CatalogueValidator::checked_trains() const {
    return trains_;
}

}

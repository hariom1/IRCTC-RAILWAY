#include "irct/report.h"

#include <cstdio>

#include "irct/catalogue_query.h"

namespace irct {

Report::Report() {}

void Report::add_line(const std::string &line) {
    lines_.push_back(line);
}

void Report::add_kv(const std::string &key, const std::string &value) {
    lines_.push_back(key + ": " + value);
}

void Report::add_kv_number(const std::string &key, long value) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%ld", value);
    lines_.push_back(key + ": " + buf);
}

void Report::add_separator() {
    lines_.push_back("----------------------------------------");
}

std::string Report::render() const {
    std::string out;
    for (size_t i = 0; i < lines_.size(); ++i) {
        out += lines_[i];
        out.push_back('\n');
    }
    return out;
}

size_t Report::line_count() const {
    return lines_.size();
}

Report Report::from_catalogue(const irct_catalogue *cat) {
    Report report;
    report.add_line("Indian Railway Catalogue Report");
    report.add_separator();

    CatalogueQuery query(cat);
    report.add_kv_number("stations", static_cast<long>(query.station_count()));
    report.add_kv_number("trains", static_cast<long>(query.train_count()));
    report.add_separator();

    std::vector<std::string> names = query.all_station_names();
    for (size_t i = 0; i < names.size(); ++i) {
        uint16_t zone = 0;
        irct_station_zone(cat, i, &zone);
        char buf[16];
        std::snprintf(buf, sizeof(buf), "zone %u", static_cast<unsigned>(zone));
        report.add_kv(names[i], buf);
    }
    return report;
}

}

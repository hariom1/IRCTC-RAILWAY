#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "irct.h"
#include "irct/builder.h"
#include "irct/catalogue_query.h"
#include "irct/csv.h"
#include "irct/diagnostics.h"
#include "irct/fare_table.h"
#include "irct/format.h"
#include "irct/report.h"
#include "irct/validator.h"

static std::vector<uint8_t> small_catalogue() {
    irct::CatalogueBuilder b;
    uint32_t n0 = b.add_name("NDLS");
    uint32_t n1 = b.add_name("CSMT");
    b.add_station(n0, 1);
    b.add_station(n1, 5);

    irct::BuildTrain train;
    train.number = 12137;
    irct::BuildStop s0;
    s0.station_index = 0;
    s0.arrival = 700;
    s0.departure = 710;
    s0.halt_kind = irct::format::kHaltCommercial;
    train.stops.push_back(s0);
    b.add_train(train);
    return b.build();
}

static void test_fare_table() {
    irct::FareTable table;
    table.load_defaults();
    assert(table.slab_count(irct::FareClass::Sleeper) == 2);
    uint32_t q = table.quote(irct::FareClass::Second, 50);
    assert(q == 3000 + 40 * 50);
    uint32_t t = table.total(irct::FareClass::Sleeper, 200);
    assert(t > table.quote(irct::FareClass::Sleeper, 200));
}

static void test_csv_roundtrip() {
    irct::CsvDocument doc;
    irct::CsvRow header;
    header.push_back("code");
    header.push_back("name");
    doc.set_header(header);
    irct::CsvRow r1;
    r1.push_back("NDLS");
    r1.push_back("New Delhi, Main");
    doc.add_row(r1);

    std::string text = doc.to_string();
    irct::CsvDocument parsed = irct::CsvDocument::parse(text);
    assert(parsed.column_count() == 2);
    assert(parsed.row_count() == 1);
    assert(parsed.row(0)[1] == "New Delhi, Main");

    size_t idx = 0;
    assert(parsed.column_index("name", &idx));
    assert(idx == 1);
}

static void test_validator() {
    std::vector<uint8_t> data = small_catalogue();
    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(data.data(), data.size(), &status);
    assert(cat != nullptr);

    irct::DiagnosticSink sink;
    irct::CatalogueValidator validator;
    bool ok = validator.validate(cat, &sink);
    assert(ok);
    assert(validator.checked_stations() == 2);
    assert(validator.checked_trains() == 1);

    irct_free(cat);
}

static void test_report() {
    std::vector<uint8_t> data = small_catalogue();
    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(data.data(), data.size(), &status);
    assert(cat != nullptr);

    irct::Report report = irct::Report::from_catalogue(cat);
    std::string text = report.render();
    assert(text.find("stations: 2") != std::string::npos);
    assert(text.find("NDLS") != std::string::npos);
    assert(report.line_count() > 4);

    irct_free(cat);
}

int main() {
    test_fare_table();
    test_csv_roundtrip();
    test_validator();
    test_report();
    std::printf("all module tests passed\n");
    return 0;
}

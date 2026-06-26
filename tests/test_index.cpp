#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "irct.h"
#include "irct/availability.h"
#include "irct/builder.h"
#include "irct/fare_table.h"
#include "irct/format.h"
#include "irct/packed_index.h"
#include "irct/station_index.h"
#include "irct/journey_planner.h"
#include "irct/ticket.h"

static std::vector<uint8_t> three_station_catalogue() {
    irct::CatalogueBuilder b;
    uint32_t n0 = b.add_name("NDLS");
    uint32_t n1 = b.add_name("BCT");
    uint32_t n2 = b.add_name("MAS");
    b.add_station(n0, 1);
    b.add_station(n1, 2);
    b.add_station(n2, 6);

    irct::BuildTrain train;
    train.number = 12621;
    irct::BuildStop s0;
    s0.station_index = 0;
    s0.arrival = 700;
    s0.departure = 720;
    s0.halt_kind = irct::format::kHaltCommercial;
    train.stops.push_back(s0);
    b.add_train(train);
    return b.build();
}

static void test_ticket() {
    irct::FareTable table;
    table.load_defaults();

    irct::Ticket ticket("2614371905", 12621);
    ticket.set_journey(0, 2, 2180);
    ticket.set_class(irct::FareClass::ThirdAc);

    irct::Passenger p1;
    p1.name = "Asha";
    p1.age = 34;
    p1.gender = 'F';
    p1.seat = "B1-22";
    p1.status = irct::BookingStatus::Confirmed;
    ticket.add_passenger(p1);

    irct::Passenger p2;
    p2.name = "Ravi";
    p2.age = 38;
    p2.gender = 'M';
    p2.seat = "B1-23";
    p2.status = irct::BookingStatus::Waitlisted;
    ticket.add_passenger(p2);

    assert(ticket.passenger_count() == 2);
    assert(ticket.confirmed_count() == 1);
    uint32_t total = ticket.fare_total(table);
    assert(total > 0);
    std::string summary = ticket.summary();
    assert(summary.find("2614371905") != std::string::npos);
}

static void test_availability() {
    irct::Availability avail;
    irct::Coach c1;
    c1.code = "B1";
    c1.cls = irct::FareClass::ThirdAc;
    c1.total_berths = 64;
    c1.booked_berths = 60;
    avail.add_coach(c1);

    irct::Coach c2;
    c2.code = "B2";
    c2.cls = irct::FareClass::ThirdAc;
    c2.total_berths = 64;
    c2.booked_berths = 64;
    avail.add_coach(c2);

    assert(avail.coach_count() == 2);
    assert(avail.open_berths(irct::FareClass::ThirdAc) == 4);
    assert(avail.is_available(irct::FareClass::ThirdAc));
    assert(avail.book(irct::FareClass::ThirdAc, 4));
    assert(avail.open_berths(irct::FareClass::ThirdAc) == 0);
    assert(!avail.book(irct::FareClass::ThirdAc, 1));
    avail.release(irct::FareClass::ThirdAc, 2);
    assert(avail.open_berths(irct::FareClass::ThirdAc) == 2);
}

static void test_station_index() {
    std::vector<uint8_t> data = three_station_catalogue();
    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(data.data(), data.size(), &status);
    assert(cat != nullptr);

    irct::StationIndex index;
    index.build(cat);
    assert(index.size() == 3);

    irct::StationRecord rec;
    assert(index.lookup_by_code("BCT", &rec));
    assert(rec.zone == 2);
    assert(!index.lookup_by_code("XXX", &rec));

    std::vector<irct::StationRecord> hits = index.prefix_search("M");
    assert(hits.size() == 1);
    assert(hits[0].code == "MAS");

    irct_free(cat);
}

static void test_packed_index() {
    std::vector<uint8_t> data = three_station_catalogue();
    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(data.data(), data.size(), &status);
    assert(cat != nullptr);

    irct::StationIndex index;
    index.build(cat);

    irct::PackedIndex packer;
    std::vector<uint8_t> blob = packer.serialize(index);
    assert(!blob.empty());

    irct::PackedIndex loaded;
    assert(loaded.deserialize(blob.data(), blob.size()));
    assert(loaded.entry_count() == 3);

    uint32_t si = 0;
    assert(loaded.find("NDLS", &si));

    uint8_t junk[4] = {1, 2, 3, 4};
    irct::PackedIndex bad;
    assert(!bad.deserialize(junk, sizeof(junk)));

    irct_free(cat);
}


static void test_journey_planner() {
    irct::RouteGraph graph(4);
    graph.add_edge(0, 1, 12001, 120);
    graph.add_edge(1, 2, 12001, 180);
    graph.add_edge(2, 3, 12002, 90);
    graph.add_edge(0, 3, 12050, 600);

    irct::FareTable fares;
    fares.load_defaults();

    irct::JourneyPlanner planner(graph, fares);
    irct::JourneyPlan plan = planner.plan(0, 3, irct::FareClass::Sleeper);
    assert(plan.found);
    assert(plan.legs.size() == 3);
    assert(plan.total_minutes == 390);
    assert(plan.total_fare > 0);

    irct::JourneyPlan none = planner.plan(3, 0, irct::FareClass::Sleeper);
    assert(!none.found);
}

int main() {
    test_ticket();
    test_availability();
    test_station_index();
    test_packed_index();
    test_journey_planner();
    std::printf("all index tests passed\n");
    return 0;
}

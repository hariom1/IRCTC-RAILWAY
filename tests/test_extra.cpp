#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <vector>
#include "irct/coach_layout.h"
#include "irct/distance_table.h"
#include "irct/fare_table.h"
#include "irct/geo.h"
#include "irct/pnr_registry.h"
#include "irct/ticket.h"
#include "irct/timetable.h"
#include "irct/text_format.h"
#include "irct/checksum_index.h"

static void test_geo() {
    irct::GeoIndex geo;
    uint32_t ndls = geo.add("NDLS", 28.6428, 77.2191);
    uint32_t csmt = geo.add("CSMT", 18.9398, 72.8355);
    double d = geo.distance_km(ndls, csmt);
    assert(d > 1100.0 && d < 1200.0);
    uint32_t near = 99;
    assert(geo.nearest(28.6, 77.2, &near));
    assert(near == ndls);
    assert(geo.size() == 2);
}

static void test_timetable() {
    irct::Timetable tt;
    irct::TimetableEntry a;
    a.train_number = 12951; a.station_index = 0; a.arrival = 0; a.departure = 1630; a.day_offset = 0;
    irct::TimetableEntry b;
    b.train_number = 12951; b.station_index = 1; b.arrival = 830; b.departure = 840; b.day_offset = 1;
    tt.add_entry(b);
    tt.add_entry(a);
    tt.sort_by_departure();
    assert(tt.at(0).day_offset == 0);
    assert(tt.entries_for_train(12951).size() == 2);
    uint32_t busy = 99;
    assert(tt.busiest_station(&busy));
}

static void test_pnr() {
    irct::PnrRegistry reg;
    irct::Ticket t("2614371905", 12621);
    irct::Passenger p;
    p.name = "Asha"; p.age = 30; p.gender = 'F'; p.seat = "B1-1"; p.status = irct::BookingStatus::Confirmed;
    t.add_passenger(p);
    assert(reg.register_ticket(t));
    assert(reg.exists("2614371905"));
    assert(!reg.register_ticket(t));
    assert(reg.cancel("2614371905"));
    assert(reg.active_count() == 0);
    assert(!irct::PnrRegistry::valid_pnr("12AB"));
}

static void test_coach() {
    irct::CoachLayout coach("B1");
    coach.generate_sleeper(9);
    assert(coach.total() == 72);
    uint16_t n = coach.first_available(irct::BerthType::Lower);
    assert(n != 0);
    assert(coach.allocate(n));
    assert(!coach.allocate(n));
    assert(coach.occupied() == 1);
}

static void test_distance() {
    irct::DistanceTable dt(4);
    std::vector<uint32_t> gaps;
    gaps.push_back(100); gaps.push_back(200); gaps.push_back(50);
    dt.build_linear(gaps);
    assert(dt.get(0, 3) == 350);
    uint32_t far = 0;
    assert(dt.farthest_from(0, &far));
    assert(far == 3);
    std::vector<uint32_t> route;
    route.push_back(0); route.push_back(1); route.push_back(2);
    assert(dt.cumulative(route) == 300);
}


static void test_text_format() {
    irct::TextTable t;
    std::vector<std::string> h;
    h.push_back("code"); h.push_back("zone");
    t.set_columns(h);
    std::vector<std::string> r;
    r.push_back("NDLS"); r.push_back("NR");
    t.add_row(r);
    std::string out = t.render();
    assert(out.find("NDLS") != std::string::npos);
    assert(t.row_count() == 1);
    assert(irct::format_minutes(135) == "2h 15m");
    assert(irct::format_rupees(12345) == "Rs 123.45");
}

static void test_checksum_index() {
    irct::ChecksumIndex idx;
    const char *a = "abc";
    const char *b = "defg";
    idx.add_block(reinterpret_cast<const uint8_t *>(a), 3);
    idx.add_block(reinterpret_cast<const uint8_t *>(b), 4);
    assert(idx.block_count() == 2);
    assert(idx.verify(0, reinterpret_cast<const uint8_t *>(a), 3));
    assert(!idx.verify(0, reinterpret_cast<const uint8_t *>(b), 4));
    assert(idx.combined() != 0);
}

int main() {
    test_geo();
    test_timetable();
    test_pnr();
    test_coach();
    test_distance();
    test_text_format();
    test_checksum_index();
    std::printf("all extra tests passed\n");
    return 0;
}

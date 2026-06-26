#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "irct.h"
#include "irct/builder.h"
#include "irct/bytebuffer.h"
#include "irct/catalogue_query.h"
#include "irct/crc32.h"
#include "irct/diagnostics.h"
#include "irct/format.h"
#include "irct/intern.h"
#include "irct/route_graph.h"
#include "irct/schedule.h"

static std::vector<uint8_t> build_valid() {
    irct::CatalogueBuilder b;
    uint32_t n0 = b.add_name("NDLS");
    uint32_t n1 = b.add_name("BCT");
    uint32_t n2 = b.add_name("HWH");
    b.add_station(n0, 1);
    b.add_station(n1, 2);
    b.add_station(n2, 3);

    irct::BuildTrain train;
    train.number = 12951;
    irct::BuildStop s0;
    s0.station_index = 0;
    s0.arrival = 600;
    s0.departure = 605;
    s0.halt_kind = irct::format::kHaltCommercial;
    irct::BuildStop s1;
    s1.station_index = 1;
    s1.arrival = 1200;
    s1.departure = 0;
    s1.halt_kind = irct::format::kHaltTerminal;
    train.stops.push_back(s0);
    train.stops.push_back(s1);
    b.add_train(train);

    return b.build();
}

static void test_parser() {
    std::vector<uint8_t> data = build_valid();
    irct_status status = IRCT_OK;
    irct_catalogue *cat = irct_parse(data.data(), data.size(), &status);
    assert(cat != nullptr);
    assert(status == IRCT_OK);

    irct::CatalogueQuery q(cat);
    assert(q.station_count() == 3);
    assert(q.train_count() == 1);

    irct::StationInfo info;
    assert(q.find_station_by_name("BCT", &info));
    assert(info.index == 1);
    assert(info.zone == 2);

    std::vector<irct::StationInfo> z3 = q.stations_in_zone(3);
    assert(z3.size() == 1);
    assert(z3[0].name == "HWH");

    irct_free(cat);

    uint8_t junk[4] = {0, 1, 2, 3};
    irct_status bad = IRCT_OK;
    assert(irct_parse(junk, sizeof(junk), &bad) == nullptr);
}

static void test_crc32() {
    const char *msg = "123456789";
    uint32_t c = irct::Crc32::compute(
        reinterpret_cast<const uint8_t *>(msg), 9);
    assert(c == 0xCBF43926u);
}

static void test_schedule() {
    irct::DayTime t = irct::Schedule::from_packed(605);
    assert(t.minutes == 365);
    assert(irct::Schedule::format(t) == "06:05");
    assert(irct::Schedule::span_minutes(
               irct::Schedule::from_packed(2330),
               irct::Schedule::from_packed(15)) == 45);
    uint16_t p = irct::Schedule::add_minutes(2350, 20);
    assert(p == 10);
}

static void test_intern() {
    irct::InternPool pool;
    uint32_t a = pool.intern("alpha");
    uint32_t b = pool.intern("beta");
    uint32_t a2 = pool.intern("alpha");
    assert(a == a2);
    assert(a != b);
    assert(pool.size() == 2);
    assert(pool.resolve(a) == "alpha");
    assert(pool.contains("beta"));
}

static void test_route_graph() {
    irct::RouteGraph g(4);
    g.add_edge(0, 1, 100, 30);
    g.add_edge(1, 2, 100, 45);
    g.add_edge(0, 2, 200, 90);
    g.add_edge(2, 3, 300, 20);
    assert(g.edge_count() == 4);
    assert(g.reachable(0, 3));
    assert(!g.reachable(3, 0));
    assert(g.shortest_minutes(0, 2) == 75);
    assert(g.shortest_minutes(0, 3) == 95);
    assert(g.degree(0) == 2);
}

static void test_bytebuffer() {
    irct::ByteBuffer buf;
    buf.put_u32(0xDEADBEEFu);
    buf.put_u16(0x1234u);
    buf.put_u8(0x56u);
    assert(buf.size() == 7);
    buf.patch_u32(0, 0x11223344u);
    const uint8_t *d = buf.data();
    assert(d[0] == 0x44);
    assert(d[3] == 0x11);
}

static void test_diagnostics() {
    irct::DiagnosticSink sink;
    sink.info(0, "loaded");
    sink.warn(10, "odd value");
    assert(!sink.has_errors());
    sink.error(20, "broken");
    assert(sink.has_errors());
    assert(sink.count() == 3);
    std::string text = sink.render();
    assert(text.find("broken") != std::string::npos);
    assert(std::strcmp(
               irct::DiagnosticSink::status_name(IRCT_ERR_MAGIC), "magic") == 0);
}

int main() {
    test_parser();
    test_crc32();
    test_schedule();
    test_intern();
    test_route_graph();
    test_bytebuffer();
    test_diagnostics();
    std::printf("all tests passed\n");
    return 0;
}

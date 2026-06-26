#!/bin/bash -eu

INCLUDES="-I$SRC/include"

SOURCES="
$SRC/src/format/builder.cpp
$SRC/src/format/csv.cpp
$SRC/src/format/packed_index.cpp
$SRC/src/irct.cpp
$SRC/src/model/alias_resolver.cpp
$SRC/src/model/amenities.cpp
$SRC/src/model/availability.cpp
$SRC/src/model/berth_allocator.cpp
$SRC/src/model/catering.cpp
$SRC/src/model/coach_layout.cpp
$SRC/src/model/composition.cpp
$SRC/src/model/concession.cpp
$SRC/src/model/concession_card.cpp
$SRC/src/model/distance_table.cpp
$SRC/src/model/fare_rules.cpp
$SRC/src/model/fare_split.cpp
$SRC/src/model/fare_table.cpp
$SRC/src/model/feedback.cpp
$SRC/src/model/geo.cpp
$SRC/src/model/holiday_calendar.cpp
$SRC/src/model/intern.cpp
$SRC/src/model/ledger.cpp
$SRC/src/model/loyalty.cpp
$SRC/src/model/luggage.cpp
$SRC/src/model/maintenance.cpp
$SRC/src/model/passenger_rules.cpp
$SRC/src/model/pnr_registry.cpp
$SRC/src/model/quota_allocator.cpp
$SRC/src/model/reservation_queue.cpp
$SRC/src/model/route_graph.cpp
$SRC/src/model/tatkal_window.cpp
$SRC/src/model/ticket.cpp
$SRC/src/model/train_class.cpp
$SRC/src/model/train_number.cpp
$SRC/src/model/waitlist.cpp
$SRC/src/model/zone_registry.cpp
$SRC/src/query/booking_engine.cpp
$SRC/src/query/catalogue_query.cpp
$SRC/src/query/chart.cpp
$SRC/src/query/itinerary.cpp
$SRC/src/query/journey_planner.cpp
$SRC/src/query/occupancy.cpp
$SRC/src/query/platform_allocator.cpp
$SRC/src/query/quote_list.cpp
$SRC/src/query/refund.cpp
$SRC/src/query/report.cpp
$SRC/src/query/running_status.cpp
$SRC/src/query/schedule_solver.cpp
$SRC/src/query/station_index.cpp
$SRC/src/query/timetable.cpp
$SRC/src/query/train_search.cpp
$SRC/src/query/validator.cpp
$SRC/src/util/bytebuffer.cpp
$SRC/src/util/calendar.cpp
$SRC/src/util/checksum_index.cpp
$SRC/src/util/crc32.cpp
$SRC/src/util/diagnostics.cpp
$SRC/src/util/money.cpp
$SRC/src/util/schedule.cpp
$SRC/src/util/text_format.cpp
"

OBJS=""
for f in $SOURCES; do
    obj="$(basename "$f" .cpp).o"
    $CXX $CXXFLAGS -std=c++14 $INCLUDES -c "$f" -o "$obj"
    OBJS="$OBJS $obj"
done

$CXX $CXXFLAGS -std=c++14 $INCLUDES \
    $LIB_FUZZING_ENGINE \
    "$SRC/fuzz/irct_fuzzer.cc" $OBJS \
    -o "$OUT/irct_fuzzer"

$CXX $CXXFLAGS -std=c++14 $INCLUDES \
    $LIB_FUZZING_ENGINE \
    "$SRC/fuzz/builder_fuzzer.cc" $OBJS \
    -o "$OUT/builder_fuzzer"

if [ -d "$SRC/fuzz/corpus/irct_fuzzer" ]; then
    (cd "$SRC/fuzz/corpus/irct_fuzzer" && zip -q -r "$OUT/irct_fuzzer_seed_corpus.zip" .)
fi
if [ -d "$SRC/fuzz/corpus/builder_fuzzer" ]; then
    (cd "$SRC/fuzz/corpus/builder_fuzzer" && zip -q -r "$OUT/builder_fuzzer_seed_corpus.zip" .)
fi

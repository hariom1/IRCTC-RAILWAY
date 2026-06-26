cmake_minimum_required(VERSION 3.10)
project(irct LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(irct STATIC
    src/format/builder.cpp
    src/format/csv.cpp
    src/format/packed_index.cpp
    src/irct.cpp
    src/model/alias_resolver.cpp
    src/model/amenities.cpp
    src/model/availability.cpp
    src/model/berth_allocator.cpp
    src/model/catering.cpp
    src/model/coach_layout.cpp
    src/model/composition.cpp
    src/model/concession.cpp
    src/model/concession_card.cpp
    src/model/distance_table.cpp
    src/model/fare_rules.cpp
    src/model/fare_split.cpp
    src/model/fare_table.cpp
    src/model/feedback.cpp
    src/model/geo.cpp
    src/model/holiday_calendar.cpp
    src/model/intern.cpp
    src/model/ledger.cpp
    src/model/loyalty.cpp
    src/model/luggage.cpp
    src/model/maintenance.cpp
    src/model/passenger_rules.cpp
    src/model/pnr_registry.cpp
    src/model/quota_allocator.cpp
    src/model/reservation_queue.cpp
    src/model/route_graph.cpp
    src/model/tatkal_window.cpp
    src/model/ticket.cpp
    src/model/train_class.cpp
    src/model/train_number.cpp
    src/model/waitlist.cpp
    src/model/zone_registry.cpp
    src/query/booking_engine.cpp
    src/query/catalogue_query.cpp
    src/query/chart.cpp
    src/query/itinerary.cpp
    src/query/journey_planner.cpp
    src/query/occupancy.cpp
    src/query/platform_allocator.cpp
    src/query/quote_list.cpp
    src/query/refund.cpp
    src/query/report.cpp
    src/query/running_status.cpp
    src/query/schedule_solver.cpp
    src/query/station_index.cpp
    src/query/timetable.cpp
    src/query/train_search.cpp
    src/query/validator.cpp
    src/util/bytebuffer.cpp
    src/util/calendar.cpp
    src/util/checksum_index.cpp
    src/util/crc32.cpp
    src/util/diagnostics.cpp
    src/util/money.cpp
    src/util/schedule.cpp
    src/util/text_format.cpp
)
target_include_directories(irct PUBLIC include)

add_executable(irct_dump src/main.cpp)
target_link_libraries(irct_dump PRIVATE irct)

enable_testing()
add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE irct)
add_test(NAME test_parser COMMAND test_parser)

add_executable(test_modules tests/test_modules.cpp)
target_link_libraries(test_modules PRIVATE irct)
add_test(NAME test_modules COMMAND test_modules)

add_executable(test_index tests/test_index.cpp)
target_link_libraries(test_index PRIVATE irct)
add_test(NAME test_index COMMAND test_index)

add_executable(test_extra tests/test_extra.cpp)
target_link_libraries(test_extra PRIVATE irct)
add_test(NAME test_extra COMMAND test_extra)


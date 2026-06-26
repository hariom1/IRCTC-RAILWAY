# IRCT: Indian Railway Catalogue Table

IRCT is a compact binary container and reference library for Indian Railway
timetable data. A single catalogue file packs a station name table, a station
list, and a set of trains, where each train carries an ordered route of stops.
The format is designed for fast, read only loading on low memory devices at
booking counters and on board display units.

This repository contains the reference parser, a set of model and query
modules built on top of it, a command line dump tool, and unit tests.

## Layout

```
include/irct/      public module headers
include/           core parser header and reader
src/util/          crc32, schedule, diagnostics, byte buffer
src/model/         interning, route graph, fares, tickets, availability
src/format/        catalogue builder, csv, packed index
src/query/         catalogue query, validator, report, station index, planner
tests/             correctness tests
fuzz/              fuzzing harnesses, seed corpus, dictionary
.clusterfuzzlite/  build entry point and metadata
```

## Modules

The library is organised in layers. The format layer reads and writes the
binary catalogue and a packed station lookup table. The model layer holds the
domain types: a string interning pool, a route graph with reachability and
shortest path, a slab based fare table, tickets with passengers, and seat
availability with quotas. The query layer sits on top and offers catalogue
lookups, a validator, a report formatter, a sorted station index with prefix
search, and a journey planner that combines the route graph, the schedule
helpers, and the fare table.

## File format

All integers are little endian.

```
Header
  u32 magic        ascii "IRCT"
  u16 version      currently 3
  u16 sections     number of sections that follow

Section (repeated)
  u8  tag
    0x10 strings
    0x20 stations
    0x30 trains
    0xff end

Strings section
  u32 count
  u32 blob_len
  u32 offset[count]    each offset points inside the blob
  u8  blob[blob_len]   null separated names

Stations section
  u32 count
  per station:
    u32 name_ref       index into the string table
    u16 zone

Trains section
  u32 count
  per train:
    u32 number
    u32 declared       number of primary stops
    per stop:
      u32 station_index
      u16 arrival
      u16 departure
      u8  halt_kind
      u8  ext_count    follow on stops appended to the route
      per extension:
        u32 ext_station
        u8  ext_kind
```

## Build

```
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Dump tool

```
./build/irct_dump path/to/catalogue.irct
```

## Fuzzing

Two harnesses ship with the project. The first in `fuzz/irct_fuzzer.cc` feeds
raw bytes straight into `irct_parse`. The second in `fuzz/builder_fuzzer.cc`
drives the catalogue builder and query path. Seed inputs live under
`fuzz/corpus`. The build entry point for ClusterFuzzLite is
`.clusterfuzzlite/build.sh`.

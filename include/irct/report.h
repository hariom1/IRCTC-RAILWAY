#ifndef IRCT_REPORT_H
#define IRCT_REPORT_H

#include <string>
#include <vector>

#include "irct.h"

namespace irct {

class Report {
public:
    Report();

    void add_line(const std::string &line);

    void add_kv(const std::string &key, const std::string &value);

    void add_kv_number(const std::string &key, long value);

    void add_separator();

    std::string render() const;

    size_t line_count() const;

    static Report from_catalogue(const irct_catalogue *cat);

private:
    std::vector<std::string> lines_;
};

}

#endif

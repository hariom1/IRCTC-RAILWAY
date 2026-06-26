#ifndef IRCT_CSV_H
#define IRCT_CSV_H

#include <cstddef>
#include <string>
#include <vector>

namespace irct {

typedef std::vector<std::string> CsvRow;

class CsvDocument {
public:
    CsvDocument();

    void set_header(const CsvRow &header);

    void add_row(const CsvRow &row);

    size_t row_count() const;

    size_t column_count() const;

    const CsvRow &header() const;

    const CsvRow &row(size_t i) const;

    std::string to_string() const;

    static CsvDocument parse(const std::string &text);

    bool column_index(const std::string &name, size_t *out) const;

private:
    static CsvRow split_line(const std::string &line);
    static std::string escape(const std::string &field);

    CsvRow header_;
    std::vector<CsvRow> rows_;
    CsvRow empty_;
};

}

#endif

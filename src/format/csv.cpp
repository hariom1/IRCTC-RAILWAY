#include "irct/csv.h"

namespace irct {

CsvDocument::CsvDocument() {}

void CsvDocument::set_header(const CsvRow &header) {
    header_ = header;
}

void CsvDocument::add_row(const CsvRow &row) {
    rows_.push_back(row);
}

size_t CsvDocument::row_count() const {
    return rows_.size();
}

size_t CsvDocument::column_count() const {
    return header_.size();
}

const CsvRow &CsvDocument::header() const {
    return header_;
}

const CsvRow &CsvDocument::row(size_t i) const {
    if (i >= rows_.size()) {
        return empty_;
    }
    return rows_[i];
}

bool CsvDocument::column_index(const std::string &name, size_t *out) const {
    for (size_t i = 0; i < header_.size(); ++i) {
        if (header_[i] == name) {
            if (out) {
                *out = i;
            }
            return true;
        }
    }
    return false;
}

std::string CsvDocument::escape(const std::string &field) {
    bool needs = false;
    for (size_t i = 0; i < field.size(); ++i) {
        char c = field[i];
        if (c == ',' || c == '"' || c == '\n' || c == '\r') {
            needs = true;
            break;
        }
    }
    if (!needs) {
        return field;
    }
    std::string out;
    out.push_back('"');
    for (size_t i = 0; i < field.size(); ++i) {
        char c = field[i];
        if (c == '"') {
            out.push_back('"');
        }
        out.push_back(c);
    }
    out.push_back('"');
    return out;
}

std::string CsvDocument::to_string() const {
    std::string out;
    for (size_t i = 0; i < header_.size(); ++i) {
        if (i > 0) {
            out.push_back(',');
        }
        out += escape(header_[i]);
    }
    out.push_back('\n');
    for (size_t r = 0; r < rows_.size(); ++r) {
        const CsvRow &row = rows_[r];
        for (size_t c = 0; c < row.size(); ++c) {
            if (c > 0) {
                out.push_back(',');
            }
            out += escape(row[c]);
        }
        out.push_back('\n');
    }
    return out;
}

CsvRow CsvDocument::split_line(const std::string &line) {
    CsvRow fields;
    std::string cur;
    bool in_quotes = false;
    size_t i = 0;
    while (i < line.size()) {
        char c = line[i];
        if (in_quotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    cur.push_back('"');
                    i += 2;
                    continue;
                }
                in_quotes = false;
                ++i;
                continue;
            }
            cur.push_back(c);
            ++i;
            continue;
        }
        if (c == '"') {
            in_quotes = true;
            ++i;
            continue;
        }
        if (c == ',') {
            fields.push_back(cur);
            cur.clear();
            ++i;
            continue;
        }
        cur.push_back(c);
        ++i;
    }
    fields.push_back(cur);
    return fields;
}

CsvDocument CsvDocument::parse(const std::string &text) {
    CsvDocument doc;
    std::vector<std::string> lines;
    std::string cur;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (c == '\n') {
            lines.push_back(cur);
            cur.clear();
            continue;
        }
        if (c == '\r') {
            continue;
        }
        cur.push_back(c);
    }
    if (!cur.empty()) {
        lines.push_back(cur);
    }
    bool first = true;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) {
            continue;
        }
        CsvRow row = split_line(lines[i]);
        if (first) {
            doc.set_header(row);
            first = false;
        } else {
            doc.add_row(row);
        }
    }
    return doc;
}

}

#include "irct/text_format.h"
#include <cstdio>
namespace irct {
TextTable::TextTable() {}
void TextTable::set_columns(const std::vector<std::string> &headers) {
    headers_ = headers;
}
void TextTable::add_row(const std::vector<std::string> &cells) {
    rows_.push_back(cells);
}
size_t TextTable::row_count() const { return rows_.size(); }
size_t TextTable::column_count() const { return headers_.size(); }
void TextTable::clear() {
    headers_.clear();
    rows_.clear();
}
std::string TextTable::pad(const std::string &s, size_t width) {
    std::string out = s;
    while (out.size() < width) {
        out.push_back(' ');
    }
    return out;
}
std::vector<size_t> TextTable::widths() const {
    std::vector<size_t> w(headers_.size(), 0);
    for (size_t i = 0; i < headers_.size(); ++i) {
        w[i] = headers_[i].size();
    }
    for (size_t r = 0; r < rows_.size(); ++r) {
        for (size_t c = 0; c < rows_[r].size() && c < w.size(); ++c) {
            if (rows_[r][c].size() > w[c]) {
                w[c] = rows_[r][c].size();
            }
        }
    }
    return w;
}
std::string TextTable::render() const {
    std::vector<size_t> w = widths();
    std::string out;
    for (size_t i = 0; i < headers_.size(); ++i) {
        if (i > 0) {
            out += " | ";
        }
        out += pad(headers_[i], w[i]);
    }
    out.push_back('\n');
    for (size_t i = 0; i < headers_.size(); ++i) {
        if (i > 0) {
            out += "-+-";
        }
        out += std::string(w[i], '-');
    }
    out.push_back('\n');
    for (size_t r = 0; r < rows_.size(); ++r) {
        for (size_t c = 0; c < w.size(); ++c) {
            if (c > 0) {
                out += " | ";
            }
            std::string cell = c < rows_[r].size() ? rows_[r][c] : "";
            out += pad(cell, w[c]);
        }
        out.push_back('\n');
    }
    return out;
}
std::string format_minutes(int minutes) {
    if (minutes < 0) {
        minutes = 0;
    }
    int h = minutes / 60;
    int m = minutes % 60;
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%dh %02dm", h, m);
    return std::string(buf);
}
std::string format_rupees(uint32_t paise) {
    uint32_t rupees = paise / 100u;
    uint32_t rem = paise % 100u;
    char buf[32];
    std::snprintf(buf, sizeof(buf), "Rs %u.%02u", rupees, rem);
    return std::string(buf);
}
std::string left_pad(const std::string &s, size_t width, char fill) {
    if (s.size() >= width) {
        return s;
    }
    std::string out(width - s.size(), fill);
    out += s;
    return out;
}
}

#ifndef IRCT_TEXT_FORMAT_H
#define IRCT_TEXT_FORMAT_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
class TextTable {
public:
    TextTable();
    void set_columns(const std::vector<std::string> &headers);
    void add_row(const std::vector<std::string> &cells);
    std::string render() const;
    size_t row_count() const;
    size_t column_count() const;
    void clear();
private:
    std::vector<size_t> widths() const;
    static std::string pad(const std::string &s, size_t width);
    std::vector<std::string> headers_;
    std::vector<std::vector<std::string> > rows_;
};
std::string format_minutes(int minutes);
std::string format_rupees(uint32_t paise);
std::string left_pad(const std::string &s, size_t width, char fill);
}
#endif

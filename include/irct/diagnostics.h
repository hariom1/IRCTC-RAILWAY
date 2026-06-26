#ifndef IRCT_DIAGNOSTICS_H
#define IRCT_DIAGNOSTICS_H

#include <string>
#include <vector>

#include "irct.h"

namespace irct {

enum class Severity {
    Info,
    Warning,
    Error
};

struct Diagnostic {
    Severity severity;
    size_t offset;
    std::string message;
};

class DiagnosticSink {
public:
    void info(size_t offset, const std::string &message);

    void warn(size_t offset, const std::string &message);

    void error(size_t offset, const std::string &message);

    bool has_errors() const;

    size_t count() const;

    const Diagnostic &at(size_t i) const;

    void clear();

    std::string render() const;

    static const char *status_name(irct_status status);

private:
    std::vector<Diagnostic> items_;
};

}

#endif

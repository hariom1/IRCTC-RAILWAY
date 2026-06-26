#include "irct/diagnostics.h"

#include <cstdio>

namespace irct {

void DiagnosticSink::info(size_t offset, const std::string &message) {
    Diagnostic d;
    d.severity = Severity::Info;
    d.offset = offset;
    d.message = message;
    items_.push_back(d);
}

void DiagnosticSink::warn(size_t offset, const std::string &message) {
    Diagnostic d;
    d.severity = Severity::Warning;
    d.offset = offset;
    d.message = message;
    items_.push_back(d);
}

void DiagnosticSink::error(size_t offset, const std::string &message) {
    Diagnostic d;
    d.severity = Severity::Error;
    d.offset = offset;
    d.message = message;
    items_.push_back(d);
}

bool DiagnosticSink::has_errors() const {
    for (size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].severity == Severity::Error) {
            return true;
        }
    }
    return false;
}

size_t DiagnosticSink::count() const {
    return items_.size();
}

const Diagnostic &DiagnosticSink::at(size_t i) const {
    return items_[i];
}

void DiagnosticSink::clear() {
    items_.clear();
}

std::string DiagnosticSink::render() const {
    std::string out;
    for (size_t i = 0; i < items_.size(); ++i) {
        const Diagnostic &d = items_[i];
        const char *tag = "info";
        if (d.severity == Severity::Warning) {
            tag = "warning";
        } else if (d.severity == Severity::Error) {
            tag = "error";
        }
        char head[64];
        std::snprintf(head, sizeof(head), "[%s @%zu] ", tag, d.offset);
        out += head;
        out += d.message;
        out += "\n";
    }
    return out;
}

const char *DiagnosticSink::status_name(irct_status status) {
    switch (status) {
        case IRCT_OK:
            return "ok";
        case IRCT_ERR_SHORT:
            return "short";
        case IRCT_ERR_MAGIC:
            return "magic";
        case IRCT_ERR_VERSION:
            return "version";
        case IRCT_ERR_SECTION:
            return "section";
        case IRCT_ERR_RANGE:
            return "range";
        case IRCT_ERR_ALLOC:
            return "alloc";
        case IRCT_ERR_FORMAT:
            return "format";
    }
    return "unknown";
}

}

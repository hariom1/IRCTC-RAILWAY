#ifndef IRCT_MAINTENANCE_H
#define IRCT_MAINTENANCE_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
enum class MaintenanceKind {
    Cleaning,
    BrakeCheck,
    WheelInspection,
    Electrical,
    Watering,
    Fumigation
};
struct MaintenanceTask {
    std::string coach_code;
    MaintenanceKind kind;
    uint32_t due_day;
    uint32_t duration_minutes;
    bool completed;
};
class MaintenanceScheduler {
public:
    MaintenanceScheduler();
    void schedule(const std::string &coach, MaintenanceKind kind, uint32_t due_day,
                  uint32_t duration_minutes);
    bool complete_next(const std::string &coach);
    std::vector<MaintenanceTask> due_on(uint32_t day) const;
    std::vector<MaintenanceTask> pending_for(const std::string &coach) const;
    uint32_t total_downtime_minutes(const std::string &coach) const;
    size_t pending_count() const;
    size_t completed_count() const;
    bool is_fit_for_service(const std::string &coach, uint32_t current_day) const;
    const char *kind_label(MaintenanceKind kind) const;
    uint32_t interval_days(MaintenanceKind kind) const;
private:
    std::vector<MaintenanceTask> tasks_;
};
}
#endif

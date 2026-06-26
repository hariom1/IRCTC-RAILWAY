#include "irct/maintenance.h"
namespace irct {
MaintenanceScheduler::MaintenanceScheduler() {}
uint32_t MaintenanceScheduler::interval_days(MaintenanceKind kind) const {
    switch (kind) {
        case MaintenanceKind::Cleaning:
            return 1;
        case MaintenanceKind::BrakeCheck:
            return 30;
        case MaintenanceKind::WheelInspection:
            return 90;
        case MaintenanceKind::Electrical:
            return 45;
        case MaintenanceKind::Watering:
            return 1;
        case MaintenanceKind::Fumigation:
            return 60;
    }
    return 30;
}
void MaintenanceScheduler::schedule(const std::string &coach,
                                    MaintenanceKind kind, uint32_t due_day,
                                    uint32_t duration_minutes) {
    MaintenanceTask task;
    task.coach_code = coach;
    task.kind = kind;
    task.due_day = due_day;
    task.duration_minutes = duration_minutes;
    task.completed = false;
    tasks_.push_back(task);
}
bool MaintenanceScheduler::complete_next(const std::string &coach) {
    MaintenanceTask *earliest = nullptr;
    for (size_t i = 0; i < tasks_.size(); ++i) {
        if (tasks_[i].coach_code != coach || tasks_[i].completed) {
            continue;
        }
        if (!earliest || tasks_[i].due_day < earliest->due_day) {
            earliest = &tasks_[i];
        }
    }
    if (!earliest) {
        return false;
    }
    earliest->completed = true;
    return true;
}
std::vector<MaintenanceTask> MaintenanceScheduler::due_on(uint32_t day) const {
    std::vector<MaintenanceTask> out;
    for (size_t i = 0; i < tasks_.size(); ++i) {
        if (!tasks_[i].completed && tasks_[i].due_day == day) {
            out.push_back(tasks_[i]);
        }
    }
    return out;
}
std::vector<MaintenanceTask> MaintenanceScheduler::pending_for(
    const std::string &coach) const {
    std::vector<MaintenanceTask> out;
    for (size_t i = 0; i < tasks_.size(); ++i) {
        if (tasks_[i].coach_code == coach && !tasks_[i].completed) {
            out.push_back(tasks_[i]);
        }
    }
    return out;
}
uint32_t MaintenanceScheduler::total_downtime_minutes(
    const std::string &coach) const {
    uint32_t total = 0;
    for (size_t i = 0; i < tasks_.size(); ++i) {
        if (tasks_[i].coach_code == coach) {
            total += tasks_[i].duration_minutes;
        }
    }
    return total;
}
size_t MaintenanceScheduler::pending_count() const {
    size_t total = 0;
    for (size_t i = 0; i < tasks_.size(); ++i) {
        if (!tasks_[i].completed) {
            ++total;
        }
    }
    return total;
}
size_t MaintenanceScheduler::completed_count() const {
    size_t total = 0;
    for (size_t i = 0; i < tasks_.size(); ++i) {
        if (tasks_[i].completed) {
            ++total;
        }
    }
    return total;
}
bool MaintenanceScheduler::is_fit_for_service(const std::string &coach,
                                              uint32_t current_day) const {
    for (size_t i = 0; i < tasks_.size(); ++i) {
        if (tasks_[i].coach_code != coach || tasks_[i].completed) {
            continue;
        }
        if (tasks_[i].due_day <= current_day) {
            return false;
        }
    }
    return true;
}
const char *MaintenanceScheduler::kind_label(MaintenanceKind kind) const {
    switch (kind) {
        case MaintenanceKind::Cleaning:
            return "cleaning";
        case MaintenanceKind::BrakeCheck:
            return "brake check";
        case MaintenanceKind::WheelInspection:
            return "wheel inspection";
        case MaintenanceKind::Electrical:
            return "electrical";
        case MaintenanceKind::Watering:
            return "watering";
        case MaintenanceKind::Fumigation:
            return "fumigation";
    }
    return "unknown";
}
}

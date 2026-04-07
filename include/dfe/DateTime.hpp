#pragma once

#include <chrono>
#include <string>

namespace dfe::datetime {

std::chrono::sys_days parseDate(const std::string& isoDate);
std::string formatDate(std::chrono::sys_days day);
int daysUntil(const std::string& isoDate);

}  // namespace dfe::datetime

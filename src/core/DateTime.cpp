#include "dfe/DateTime.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace dfe::datetime {

std::chrono::sys_days parseDate(const std::string& isoDate) {
  std::istringstream ss(isoDate);
  int y = 0;
  int m = 0;
  int d = 0;
  char sep1 = 0;
  char sep2 = 0;
  ss >> y >> sep1 >> m >> sep2 >> d;
  if (!ss || sep1 != '-' || sep2 != '-') {
    throw std::invalid_argument("Invalid date format, expected YYYY-MM-DD");
  }
  return std::chrono::year(y) / std::chrono::month(m) / std::chrono::day(d);
}

std::string formatDate(std::chrono::sys_days day) {
  const std::chrono::year_month_day ymd{day};
  std::ostringstream out;
  out << std::setfill('0') << std::setw(4) << static_cast<int>(ymd.year()) << '-'
      << std::setw(2) << static_cast<unsigned>(ymd.month()) << '-' << std::setw(2)
      << static_cast<unsigned>(ymd.day());
  return out.str();
}

int daysUntil(const std::string& isoDate) {
  const auto target = parseDate(isoDate);
  const auto now = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
  return static_cast<int>((target - now).count());
}

}  // namespace dfe::datetime

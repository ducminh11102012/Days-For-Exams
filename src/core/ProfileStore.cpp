#include "dfe/ProfileStore.hpp"

#include <fstream>
#include <sstream>

namespace dfe {

namespace {
std::string join(const std::vector<std::string>& values) {
  std::ostringstream out;
  for (size_t i = 0; i < values.size(); ++i) {
    if (i > 0) {
      out << ',';
    }
    out << values[i];
  }
  return out.str();
}

std::vector<std::string> split(const std::string& value) {
  std::vector<std::string> result;
  std::istringstream in(value);
  std::string token;
  while (std::getline(in, token, ',')) {
    if (!token.empty()) {
      result.push_back(token);
    }
  }
  return result;
}
}  // namespace

bool ProfileStore::save(const std::filesystem::path& filePath, const UserProfile& profile) const {
  std::ofstream out(filePath);
  if (!out) {
    return false;
  }

  out << "userName=" << profile.userName << '\n';
  out << "examDate=" << profile.examDate << '\n';
  out << "dailyEndTime=" << profile.dailyEndTime << '\n';
  out << "allowedApps=" << join(profile.allowedApps) << '\n';
  out << "allowedWebsites=" << join(profile.allowedWebsites) << '\n';
  for (const auto& goal : profile.goals) {
    out << "goal=" << goal.subject << '|' << goal.priority << '|' << goal.estimatedHours << '\n';
  }

  return true;
}

std::optional<UserProfile> ProfileStore::load(const std::filesystem::path& filePath) const {
  std::ifstream in(filePath);
  if (!in) {
    return std::nullopt;
  }

  UserProfile profile;
  std::string line;
  while (std::getline(in, line)) {
    const auto pos = line.find('=');
    if (pos == std::string::npos) {
      continue;
    }

    const auto key = line.substr(0, pos);
    const auto value = line.substr(pos + 1);
    if (key == "userName") {
      profile.userName = value;
    } else if (key == "examDate") {
      profile.examDate = value;
    } else if (key == "dailyEndTime") {
      profile.dailyEndTime = value;
    } else if (key == "allowedApps") {
      profile.allowedApps = split(value);
    } else if (key == "allowedWebsites") {
      profile.allowedWebsites = split(value);
    } else if (key == "goal") {
      std::istringstream goalIn(value);
      std::string subject;
      std::string priority;
      std::string hours;
      if (std::getline(goalIn, subject, '|') && std::getline(goalIn, priority, '|') &&
          std::getline(goalIn, hours, '|')) {
        profile.goals.push_back({subject, std::stoi(priority), std::stoi(hours)});
      }
    }
  }

  if (profile.userName.empty() || profile.examDate.empty()) {
    return std::nullopt;
  }

  return profile;
}

}  // namespace dfe

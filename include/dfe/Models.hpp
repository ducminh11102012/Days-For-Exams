#pragma once

#include <string>
#include <vector>

namespace dfe {

struct StudyGoal {
  std::string subject;
  int priority{1};
  int estimatedHours{1};
};

struct StudyBlock {
  std::string startsAt;
  std::string endsAt;
  std::string subject;
  std::string task;
};

struct UserProfile {
  std::string userName;
  std::string examDate;
  std::string dailyEndTime{"23:30"};
  std::vector<StudyGoal> goals;
  std::vector<std::string> allowedApps;
  std::vector<std::string> allowedWebsites;
};

}  // namespace dfe

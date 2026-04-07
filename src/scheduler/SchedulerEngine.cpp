#include "dfe/SchedulerEngine.hpp"

#include <algorithm>
#include <chrono>

#include "dfe/DateTime.hpp"

namespace dfe {

std::vector<StudyBlock> SchedulerEngine::buildPlan(const UserProfile& profile) const {
  std::vector<StudyBlock> plan;
  if (profile.goals.empty()) {
    return plan;
  }

  const int daysLeft = std::max(1, datetime::daysUntil(profile.examDate));
  auto goals = profile.goals;
  std::sort(goals.begin(), goals.end(),
            [](const StudyGoal& a, const StudyGoal& b) { return a.priority > b.priority; });

  const auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
  int goalIndex = 0;
  for (int d = 0; d < daysLeft; ++d) {
    const auto day = today + std::chrono::days(d);
    const auto date = datetime::formatDate(day);

    for (int blockIndex = 0; blockIndex < 3; ++blockIndex) {
      const auto& goal = goals[goalIndex % goals.size()];
      const int startHour = 8 + blockIndex * 2;
      const int endHour = startHour + 2;
      StudyBlock block;
      block.startsAt = date + " " + (startHour < 10 ? "0" : "") + std::to_string(startHour) + ":00";
      block.endsAt = date + " " + (endHour < 10 ? "0" : "") + std::to_string(endHour) + ":00";
      block.subject = goal.subject;
      block.task = "Deep work session";
      plan.push_back(block);
      ++goalIndex;
    }
  }

  return plan;
}

}  // namespace dfe

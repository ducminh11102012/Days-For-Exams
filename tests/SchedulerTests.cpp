#include <cassert>
#include <iostream>

#include "dfe/SchedulerEngine.hpp"

int main() {
  dfe::UserProfile profile;
  profile.userName = "test";
  profile.examDate = "2099-01-01";
  profile.goals = {{"Math", 2, 10}, {"Chem", 1, 5}};

  dfe::SchedulerEngine engine;
  const auto plan = engine.buildPlan(profile);

  assert(!plan.empty());
  assert(plan.front().subject == "Math");

  std::cout << "Scheduler tests passed\n";
  return 0;
}

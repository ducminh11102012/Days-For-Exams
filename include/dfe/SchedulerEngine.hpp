#pragma once

#include <vector>

#include "dfe/Models.hpp"

namespace dfe {

class SchedulerEngine {
 public:
  std::vector<StudyBlock> buildPlan(const UserProfile& profile) const;
};

}  // namespace dfe

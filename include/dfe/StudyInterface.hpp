#pragma once

#include <vector>

#include "dfe/Models.hpp"

namespace dfe {

class StudyInterface {
 public:
  void runSession(const UserProfile& profile, const std::vector<StudyBlock>& plan) const;
};

}  // namespace dfe

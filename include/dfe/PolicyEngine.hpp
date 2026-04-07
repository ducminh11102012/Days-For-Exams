#pragma once

#include <string>

#include "dfe/Models.hpp"

namespace dfe {

class PolicyEngine {
 public:
  explicit PolicyEngine(UserProfile profile);

  bool isAppAllowed(const std::string& processName) const;
  bool isWebsiteAllowed(const std::string& url) const;

 private:
  UserProfile profile_;
};

}  // namespace dfe

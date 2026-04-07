#include "dfe/PolicyEngine.hpp"

#include <algorithm>

namespace dfe {

PolicyEngine::PolicyEngine(UserProfile profile) : profile_(std::move(profile)) {}

bool PolicyEngine::isAppAllowed(const std::string& processName) const {
  return std::find(profile_.allowedApps.begin(), profile_.allowedApps.end(), processName) !=
         profile_.allowedApps.end();
}

bool PolicyEngine::isWebsiteAllowed(const std::string& url) const {
  return std::any_of(profile_.allowedWebsites.begin(), profile_.allowedWebsites.end(),
                     [&url](const std::string& allowed) { return url.find(allowed) != std::string::npos; });
}

}  // namespace dfe

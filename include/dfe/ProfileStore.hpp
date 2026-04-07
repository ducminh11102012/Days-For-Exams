#pragma once

#include <filesystem>
#include <optional>

#include "dfe/Models.hpp"

namespace dfe {

class ProfileStore {
 public:
  bool save(const std::filesystem::path& filePath, const UserProfile& profile) const;
  std::optional<UserProfile> load(const std::filesystem::path& filePath) const;
};

}  // namespace dfe

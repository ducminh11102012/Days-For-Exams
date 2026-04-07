#pragma once

#include <atomic>
#include <thread>

#include "dfe/PolicyEngine.hpp"

namespace dfe {

class LockdownManager {
 public:
  explicit LockdownManager(PolicyEngine policy);
  ~LockdownManager();

  void start();
  void stop();

 private:
  void monitorLoop();

  PolicyEngine policy_;
  std::atomic<bool> running_{false};
  std::thread worker_;
};

}  // namespace dfe

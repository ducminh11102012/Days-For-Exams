#include "dfe/LockdownManager.hpp"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_set>

#ifdef _WIN32
#include <tlhelp32.h>
#include <windows.h>
#endif

namespace dfe {

namespace {
std::string lower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return value;
}

bool isDistractionProcess(const std::string& processName) {
  static const std::unordered_set<std::string> kKnownDistractors = {
      "steam.exe", "epicgameslauncher.exe", "discord.exe", "spotify.exe", "vlc.exe",
      "telegram.exe", "whatsapp.exe",           "battle.net.exe", "riotclientservices.exe"};
  return kKnownDistractors.find(lower(processName)) != kKnownDistractors.end();
}
}  // namespace

LockdownManager::LockdownManager(PolicyEngine policy) : policy_(std::move(policy)) {}

LockdownManager::~LockdownManager() { stop(); }

void LockdownManager::start() {
  if (running_) {
    return;
  }
  running_ = true;
  worker_ = std::thread(&LockdownManager::monitorLoop, this);
}

void LockdownManager::stop() {
  if (!running_) {
    return;
  }
  running_ = false;
  if (worker_.joinable()) {
    worker_.join();
  }
}

void LockdownManager::monitorLoop() {
  while (running_) {
#ifdef _WIN32
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
      PROCESSENTRY32 entry;
      entry.dwSize = sizeof(entry);
      if (Process32First(snapshot, &entry)) {
        do {
          std::string processName = entry.szExeFile;
          if (!isDistractionProcess(processName)) {
            continue;
          }

          if (!policy_.isAppAllowed(processName)) {
            HANDLE proc = OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);
            if (proc) {
              TerminateProcess(proc, 1);
              CloseHandle(proc);
            }
          }
        } while (Process32Next(snapshot, &entry));
      }
      CloseHandle(snapshot);
    }
#else
    std::cout << "[DFE] Lockdown monitor active (stub on non-Windows).\n";
#endif
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

}  // namespace dfe
